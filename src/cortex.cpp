#include <stdio.h>

#include <GL/gl.h>

#include "cortex.h"
#include "cortex_platform.h"
#include "cortex_math.h"

#include "cortex_opengl.cpp"

void InitializeArena(memory_arena *arena, size_t size, uint8 *storage)
{
    arena->base = storage;
    arena->size = size;
    arena->used = 0;
}

void *PushSize_(memory_arena *arena, size_t size)
{
    Assert(arena->size > size + arena->used);
    uint8 *ptr = arena->base + arena->used;
    arena->used += size;
    return ptr;
}

#define PushStruct(arena, type) (type *)PushSize_(arena, sizeof(type))
#define PushArray(arena, count, type) (type *)PushSize_(arena, (count) * sizeof(type))

opengl_buffer CreateOpenGLBuffer(game_state *gameState, GLenum type, uint32 capacity)
{
    opengl_buffer result = {};
    
    glCreateBuffers(1, &result.handle);
    glBindBuffer(type, result.handle);
    glBufferData(type, capacity, NULL, GL_DYNAMIC_DRAW);

    result.bufferCapacity = capacity;
    result.buffer = (real32 *) PushSize_(&gameState->permanentArena, capacity);

    return result;
}

inline void PushRenderCommand(game_state *gameState, render_command cmd)
{
    gameState->renderStack[gameState->renderStackCount++] = cmd;
}

inline void PushClear(game_state *gameState, v4 color)
{
    render_command cmd = {};
    cmd.type = RenderCommandType_Clear;
    cmd.color = color;

    PushRenderCommand(gameState, cmd);
}

inline void PushViewport(game_state *gameState, v2 min, v2 dim)
{
    render_command cmd = {};
    cmd.type = RenderCommandType_Viewport;
    cmd.rect.min = min;
    cmd.rect.max = dim;

    PushRenderCommand(gameState, cmd);
}

extern "C" GAME_UPDATE(GameUpdate)
{
    game_state *gameState = (game_state *)memory->permanentStorage;
    if (!memory->isInitialized)
    {
        InitializeArena(&gameState->permanentArena, memory->permanentStorageSize - sizeof(game_state), (uint8 *)memory->permanentStorage + sizeof(game_state));

        gameState->renderStack = PushArray(&gameState->permanentArena, 1024, render_command);
        gameState->renderStackCount = 0;

        LoadAllOpenGLFunctions(memory->getOpenGLFunction);

        glCreateVertexArrays(1, &gameState->rectVao);
        glBindVertexArray(gameState->rectVao);

        gameState->rectBuffer = CreateOpenGLBuffer(gameState, GL_ARRAY_BUFFER, 100);

        CreateOpenGLShaders(gameState->shaders);

        memory->isInitialized = true;
    }

    gameState->renderStackCount = 0;
    
    PushClear(gameState, V4(0.0f, 0.1f, 0.3f, 1.0f));
    PushViewport(gameState, V2(0.0f, 0.0f), V2(1280.0f, 720.0f));

    for (uint32 i = 0; i < gameState->renderStackCount; i++)
    {
        render_command *cmd = gameState->renderStack + i;
        switch (cmd->type)
        {
            case RenderCommandType_Clear:
            {
                glClearColor(cmd->color.x, cmd->color.y, cmd->color.z, cmd->color.w);
                glClear(GL_COLOR_BUFFER_BIT);
            } break;

            case RenderCommandType_Viewport:
            {
                glViewport(cmd->rect.min.x, cmd->rect.min.y, cmd->rect.max.x, cmd->rect.max.y);
            } break;

            case RenderCommandType_Rect:
            {
                
            } break;

            default: {}
        }
    }
}