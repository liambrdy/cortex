#include <stdio.h>

//#include <GL/GL.h>

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

void PushV2ToBuffer(opengl_buffer *buf, v2 data)
{
    buf->buffer[buf->bufferSize++] = data.x;
    buf->buffer[buf->bufferSize++] = data.y;
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
    cmd.rect.x = min.x;
    cmd.rect.y = min.y;
    cmd.rect.z = dim.x;
    cmd.rect.w = dim.y;

    PushRenderCommand(gameState, cmd);
}

inline void PushRect(game_state *gameState, v4 rect)
{
    render_command cmd = {};
    cmd.type = RenderCommandType_Rect;
    cmd.rect = rect;

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
        CreateOpenGLShaders(gameState->shaders);

        glCreateVertexArrays(1, &gameState->rectVao);
        glBindVertexArray(gameState->rectVao);

        gameState->rectBuffer = CreateOpenGLBuffer(gameState, GL_ARRAY_BUFFER, 100);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(real32), 0);

        memory->isInitialized = true;
    }

    gameState->renderStackCount = 0;
    
    PushClear(gameState, V4(0.0f, 0.1f, 0.3f, 1.0f));
    PushViewport(gameState, V2(0.0f, 0.0f), V2(1280.0f, 720.0f));

    PushRect(gameState, V4(0.0f, 0.0f, 0.5f, 0.5f));

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
                glViewport((int) cmd->rect.x, (int) cmd->rect.y, (int) cmd->rect.z, (int) cmd->rect.w);
            } break;

            case RenderCommandType_Rect:
            {
                v2 min = {cmd->rect.x, cmd->rect.y};
                v2 dim = {cmd->rect.z, cmd->rect.w};
                PushV2ToBuffer(&gameState->rectBuffer, min);
                PushV2ToBuffer(&gameState->rectBuffer, V2(min.x + dim.x, min.y));
                PushV2ToBuffer(&gameState->rectBuffer, min + dim);
                PushV2ToBuffer(&gameState->rectBuffer, min + dim);
                PushV2ToBuffer(&gameState->rectBuffer, V2(min.x, min.y + dim.y));
                PushV2ToBuffer(&gameState->rectBuffer, min);
            } break;

            default: {}
        }
    }

    if (gameState->rectBuffer.bufferSize > 0)
    {
        glBindVertexArray(gameState->rectVao);
        glBindBuffer(GL_ARRAY_BUFFER, gameState->rectBuffer.handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gameState->rectBuffer.bufferSize * sizeof(real32), gameState->rectBuffer.buffer);
        glUseProgram(gameState->shaders[ShaderType_Rect]);
        glDrawArrays(GL_TRIANGLES, 0, gameState->rectBuffer.bufferSize / 2);
        gameState->rectBuffer.bufferSize = 0;
    }
}