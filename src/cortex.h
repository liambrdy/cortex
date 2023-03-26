#ifndef __CORTEX_H
#define __CORTEX_H

#include "cortex_platform.h"
#include "cortex_math.h"

struct memory_arena
{
    size_t size;
    uint8 *base;
    size_t used;
};

enum render_command_type
{
    RenderCommandType_Clear,
    RenderCommandType_Viewport,
    RenderCommandType_Line,
    RenderCommandType_Rect,
    RenderCommandType_FilledRect,
    RenderCommandType_Texture
};

struct render_command
{
    render_command_type type;
    v4 color;
    v4 rect;
};

struct opengl_buffer
{
    uint32 handle;
    real32 *buffer;
    uint32 bufferSize;
    uint32 bufferCapacity;
};

enum shader_type
{
    ShaderType_Rect = 0,
    ShaderType_Lights,
    ShaderType_Final,
    ShaderTypeCount
};

struct render_target
{
    uint32 handle;
    uint32 targets[4];
};

struct light
{
    v2 position;
    real32 minAngle;
    real32 maxAngle;
    v4 tint;
    real32 intensity;
    v2 padding00;
    real32 padding01;
};

#define MAX_LIGHTS 16
struct game_state
{
    memory_arena permanentArena;

    render_command *renderStack;
    uint32 renderStackCount;

    m4 perspective;

    uint32 rectVao;
    opengl_buffer rectBuffer;

    uint32 shaders[ShaderTypeCount];

    render_target gBuffer;
    render_target lightTarget;

    light *lights;
    uint32 lightCount;

    uint32 lightShaderBuffer;
    uint32 lightVao;
    opengl_buffer lightVertexBuffer;

    real32 metersToPixels;
};

#endif