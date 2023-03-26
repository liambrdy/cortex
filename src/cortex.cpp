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

void PushV4ToBuffer(opengl_buffer *buf, v4 data)
{
    buf->buffer[buf->bufferSize++] = data.x;
    buf->buffer[buf->bufferSize++] = data.y;
    buf->buffer[buf->bufferSize++] = data.z;
    buf->buffer[buf->bufferSize++] = data.w;
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

inline void PushRect(game_state *gameState, v4 rect, v4 color)
{
    render_command cmd = {};
    cmd.type = RenderCommandType_Rect;
    cmd.rect = rect;
    cmd.color = color;

    PushRenderCommand(gameState, cmd);
}

internal void AddRenderTexture(render_target *target, uint32 attachment, uint32 width, uint32 height)
{
    uint32 texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, texture, 0);

    target->targets[attachment] = texture;
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

        gameState->rectBuffer = CreateOpenGLBuffer(gameState, GL_ARRAY_BUFFER, 1024*1024);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(real32), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(real32), (void *)(2 * sizeof(real32)));

        glCreateFramebuffers(1, &gameState->gBuffer.handle);
        glBindFramebuffer(GL_FRAMEBUFFER, gameState->gBuffer.handle);

        AddRenderTexture(&gameState->gBuffer, 0, input->windowWidth, input->windowHeight);
        AddRenderTexture(&gameState->gBuffer, 1, input->windowWidth, input->windowHeight);

        GLenum gDrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT0 + 1};
        glDrawBuffers(ArrayCount(gDrawBuffers), gDrawBuffers);

        glCreateFramebuffers(1, &gameState->lightTarget.handle);
        glBindFramebuffer(GL_FRAMEBUFFER, gameState->lightTarget.handle);

        AddRenderTexture(&gameState->lightTarget, 0, input->windowWidth, input->windowHeight);

        GLenum lDrawBuffers[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(ArrayCount(lDrawBuffers), lDrawBuffers);

        gameState->lights = PushArray(&gameState->permanentArena, MAX_LIGHTS, light);
        gameState->lights[gameState->lightCount++] = {
            V2(-2.0f, -0.5f),
            0, // min
            0, // max
            V4(0.9f, 0.2f, 0.2f, 1.0f),
            1.0f,
            V2(0.0f, 0.0f),
            0.0f
        };

        glCreateBuffers(1, &gameState->lightShaderBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, gameState->lightShaderBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, gameState->lightCount * sizeof(light), gameState->lights, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &gameState->lightVao);
        glBindVertexArray(gameState->lightVao);

        gameState->lightVertexBuffer = CreateOpenGLBuffer(gameState, GL_ARRAY_BUFFER, 16 * 4 * 2);
        glBindBuffer(GL_ARRAY_BUFFER, gameState->lightVertexBuffer.handle);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(real32), 0);

        real32 radius = 8.0f;
        light *l = gameState->lights;
        PushV2ToBuffer(&gameState->lightVertexBuffer, l->position + V2(-radius, -radius));
        PushV2ToBuffer(&gameState->lightVertexBuffer, l->position + V2(-radius, radius));
        PushV2ToBuffer(&gameState->lightVertexBuffer, l->position + V2(radius, -radius));
        PushV2ToBuffer(&gameState->lightVertexBuffer, l->position + V2(radius, radius));

        glBufferSubData(GL_ARRAY_BUFFER, 0, gameState->lightVertexBuffer.bufferSize * sizeof(real32), gameState->lightVertexBuffer.buffer);

        real32 aspect = (real32) input->windowWidth / (real32) input->windowHeight;
        real32 metersToPixels = 10.0f;
        gameState->perspective = M4Orthographic(-aspect * metersToPixels, aspect * metersToPixels, -metersToPixels, metersToPixels, -100.0f, 100.0f);
        gameState->metersToPixels = metersToPixels;

        memory->isInitialized = true;
    }

    gameState->renderStackCount = 0;
    
    //PushClear(gameState, V4(0.0f, 0.1f, 0.3f, 1.0f));
    PushViewport(gameState, V2(0.0f, 0.0f), V2((real32) input->windowWidth, (real32) input->windowHeight));

    // real32 metersToPixels = gameState->metersToPixels;
    // real32 aspect = 1280.0f / 720.0f;
    // for (real32 x = -metersToPixels*aspect; x < aspect*metersToPixels; x++)
    // {
    //     for (real32 y = -metersToPixels; y < metersToPixels; y++)
    //     {
    //         real32 u = (x+metersToPixels)/(2*metersToPixels);
    //         real32 v = (y+metersToPixels)/(2*metersToPixels);
    //         PushRect(gameState, V4(x, y, 1, 1), V4(u, v, 0.0f, 1.0f));
    //     }
    // }

    PushRect(gameState, V4(0.0f, 0.0f, 1.0f, 1.0f), V4(1.0f, 0.2f, 0.2f, 1.0f));

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
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
                PushV2ToBuffer(&gameState->rectBuffer, V2(min.x + dim.x, min.y));
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
                PushV2ToBuffer(&gameState->rectBuffer, min + dim);
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
                PushV2ToBuffer(&gameState->rectBuffer, min + dim);
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
                PushV2ToBuffer(&gameState->rectBuffer, V2(min.x, min.y + dim.y));
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
                PushV2ToBuffer(&gameState->rectBuffer, min);
                PushV4ToBuffer(&gameState->rectBuffer, cmd->color);
            } break;

            default: {}
        }
    }

    if (gameState->rectBuffer.bufferSize > 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gameState->gBuffer.handle);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);   
        glBindVertexArray(gameState->rectVao);
        glBindBuffer(GL_ARRAY_BUFFER, gameState->rectBuffer.handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gameState->rectBuffer.bufferSize * sizeof(real32), gameState->rectBuffer.buffer);
        glUseProgram(gameState->shaders[ShaderType_Rect]);
        glUniformMatrix4fv(glGetUniformLocation(gameState->shaders[ShaderType_Rect], "projection"), 1, GL_FALSE, &gameState->perspective.e[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, gameState->rectBuffer.bufferSize / 6);
        gameState->rectBuffer.bufferSize = 0;
    }

    if (gameState->lightCount > 0)
    {
        uint32 program = gameState->shaders[ShaderType_Lights];
        glBindFramebuffer(GL_FRAMEBUFFER, gameState->lightTarget.handle);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(gameState->lightVao);
        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &gameState->perspective.e[0][0]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gameState->lightShaderBuffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gameState->gBuffer.targets[1]);
        glUniform1i(glGetUniformLocation(program, "normalTexture"), 0);
        glUniform2f(glGetUniformLocation(program, "resolution"), (real32) input->windowWidth, (real32) input->windowHeight);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * gameState->lightCount);
    }

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, gameState->gBuffer.handle);
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    uint32 program = gameState->shaders[ShaderType_Final];
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameState->gBuffer.targets[0]);
    glUniform1i(glGetUniformLocation(program, "colorTexture"), 0);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, gameState->lightTarget.targets[0]);
    glUniform1i(glGetUniformLocation(program, "lightTexture"), 1);

    glUniform1f(glGetUniformLocation(program, "volumetricIntensity"), 1.0f);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    GLenum error = glGetError();
    if (error)
    {
        printf("[OpenGL Error] %i\n", (int) error);
    }
}