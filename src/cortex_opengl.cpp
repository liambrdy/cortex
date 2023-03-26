//#include <GL/gl.h>

#include "cortex.h"
#include "cortex_platform.h"

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

typedef char GLchar;
typedef size_t GLsizeiptr;
typedef intptr_t GLintptr;

#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ARRAY_BUFFER 0x8892
#define GL_FALSE 0
#define GL_FLOAT 0x1406
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TEXTURE_2D 0x0DE1
#define GL_RGB 0x1907
#define GL_UNSIGNED_BYTE 0x1401
#define GL_FRAMEBUFFER 0x8D40
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_STATIC_DRAW 0x88E4
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_CONTEXT_FLAGS 0x821E
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_TEXTURE0 0x84C0

extern "C" void glDrawArrays(GLenum mode, GLint first, GLsizei count);
extern "C" void glClear(GLbitfield mask);
extern "C" void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern "C" void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
extern "C" void glBindTexture(GLenum target, GLuint texture);
extern "C" void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
extern "C" void glTexParameteri(GLenum target, GLenum pname, GLint param);
extern "C" GLenum glGetError();
extern "C" void glGetIntegerv(GLenum pname, GLint *params);

typedef void (*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#define GL_FUN(name, type_ret, type, ...) \
typedef type_ret type(__VA_ARGS__); \
global type *name;

GL_FUN(glCreateVertexArrays, void, gl_create_vertex_arrays, GLsizei n, GLuint *arrays);
GL_FUN(glBindVertexArray, void, gl_bind_vertex_array, GLuint array);
GL_FUN(glCreateBuffers, void, gl_create_buffers, GLsizei n, GLuint *buffers);
GL_FUN(glBindBuffer, void, gl_bind_buffer, GLenum target, GLuint buffer);
GL_FUN(glBufferData, void, gl_buffer_data, GLenum target, GLsizeiptr size, const void * data, GLenum usage);
GL_FUN(glCreateShader, GLuint, gl_create_shader, GLenum shaderType);
GL_FUN(glDeleteShader, void, gl_delete_shader, GLuint shader);
GL_FUN(glCreateProgram, GLuint, gl_create_program);
GL_FUN(glShaderSource, void, gl_shader_source, GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
GL_FUN(glCompileShader, void, gl_compile_shader, GLuint shader);
GL_FUN(glGetShaderiv, void, gl_get_shader_iv, GLuint shader, GLenum pname, GLint *params);
GL_FUN(glGetProgramiv, void, gl_get_program_iv, GLuint program, GLenum pname, GLint *params);
GL_FUN(glGetShaderInfoLog, void, gl_get_shader_info_log, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
GL_FUN(glGetProgramInfoLog, void, gl_get_program_info_log, GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
GL_FUN(glAttachShader, void, gl_attach_shader, GLuint program, GLuint shader);
GL_FUN(glLinkProgram, void, gl_link_program, GLuint program);
GL_FUN(glBufferSubData, void, gl_buffer_sub_data, GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
GL_FUN(glUseProgram, void, gl_use_program, GLuint program);
GL_FUN(glValidateProgram, void, gl_validate_program, GLuint program);
GL_FUN(glEnableVertexAttribArray, void, gl_enable_vertex_attrib_array, GLuint index);
GL_FUN(glVertexAttribPointer, void, gl_vertex_attrib_pointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
GL_FUN(glCreateTextures, void, gl_create_textures, GLenum target, GLsizei n, GLuint *textures);
//GL_FUN(glBindTexture, void, gl_bind_texture, GLenum target, GLuint texture);
GL_FUN(glCreateFramebuffers, void, gl_create_framebuffers, GLsizei n, GLuint *framebuffers);
GL_FUN(glBindFramebuffer, void, gl_bind_framebuffer, GLenum target, GLuint framebuffer);
//GL_FUN(glTexImage2D, void, gl_tex_image_2d, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
GL_FUN(glFramebufferTexture, void, gl_framebuffer_texture, GLenum target, GLenum attachment, GLuint texture, GLint level);
GL_FUN(glDrawBuffers, void, gl_draw_buffers, GLsizei n, const GLenum *bufs);
GL_FUN(glBlitFramebuffer, void, gl_blit_framebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GL_FUN(glGetUniformLocation, GLint, gl_get_uniform_location, GLuint program, const GLchar *name);
GL_FUN(glUniformMatrix4fv, void, gl_uniform_matrix_4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GL_FUN(glBindBufferBase, void, gl_bind_buffer_base, GLenum target, GLuint index, GLuint buffer);
GL_FUN(glUniform1f, void, gl_uniform_1f, GLint location, GLfloat v0);
GL_FUN(glUniform2f, void, gl_uniform_2f, GLint location, GLfloat v0, GLfloat v1);
GL_FUN(glUniform1i, void, gl_uniform_1i, GLint location, GLint v0);
GL_FUN(glDebugMessageCallback, void, gl_debug_message_callback, DEBUGPROC callback, const void *userdata);
GL_FUN(glActiveTexture, void, gl_active_texture, GLenum texture);

#undef GL_FUN

internal void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    Unused(source);
    Unused(id);
    Unused(length);
    Unused(userParam);

    printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

internal void LoadAllOpenGLFunctions(platform_get_opengl_function *getOpenGLFunction)
{
    if (getOpenGLFunction)
    {
#define GL_FUN(name, type) \
        name = (type *)getOpenGLFunction(#name);
        GL_FUN(glCreateVertexArrays, gl_create_vertex_arrays);
        GL_FUN(glBindVertexArray, gl_bind_vertex_array);
        GL_FUN(glCreateBuffers, gl_create_buffers);
        GL_FUN(glBindBuffer, gl_bind_buffer);
        GL_FUN(glBufferData, gl_buffer_data);
        GL_FUN(glCreateShader, gl_create_shader);
        GL_FUN(glDeleteShader, gl_delete_shader);
        GL_FUN(glCreateProgram, gl_create_program);
        GL_FUN(glShaderSource, gl_shader_source);
        GL_FUN(glCompileShader, gl_compile_shader);
        GL_FUN(glGetShaderiv, gl_get_shader_iv);
        GL_FUN(glGetProgramiv, gl_get_program_iv);
        GL_FUN(glGetShaderInfoLog, gl_get_shader_info_log);
        GL_FUN(glGetProgramInfoLog, gl_get_program_info_log);
        GL_FUN(glAttachShader, gl_attach_shader);
        GL_FUN(glLinkProgram, gl_link_program);
        GL_FUN(glBufferSubData, gl_buffer_sub_data);
        GL_FUN(glUseProgram, gl_use_program);
        GL_FUN(glValidateProgram, gl_validate_program);
        GL_FUN(glEnableVertexAttribArray, gl_enable_vertex_attrib_array);
        GL_FUN(glVertexAttribPointer, gl_vertex_attrib_pointer);
        GL_FUN(glCreateTextures, gl_create_textures);
        //GL_FUN(glBindTexture, gl_bind_texture);
        GL_FUN(glCreateFramebuffers, gl_create_framebuffers);
        GL_FUN(glBindFramebuffer, gl_bind_framebuffer);
        //GL_FUN(glTexImage2D, gl_tex_image_2d);
        GL_FUN(glFramebufferTexture, gl_framebuffer_texture);
        GL_FUN(glDrawBuffers, gl_draw_buffers);
        GL_FUN(glBlitFramebuffer, gl_blit_framebuffer);
        GL_FUN(glGetUniformLocation, gl_get_uniform_location);
        GL_FUN(glUniformMatrix4fv, gl_uniform_matrix_4fv);
        GL_FUN(glBindBufferBase, gl_bind_buffer_base);
        GL_FUN(glUniform1f, gl_uniform_1f);
        GL_FUN(glUniform2f, gl_uniform_2f);
        GL_FUN(glUniform1i, gl_uniform_1i);
        GL_FUN(glDebugMessageCallback, gl_debug_message_callback);
        GL_FUN(glActiveTexture, gl_active_texture);
#undef GL_FUN
    }

    int32 context;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context);

    if (glDebugMessageCallback && (context & GL_CONTEXT_FLAG_DEBUG_BIT))
    {
        glDebugMessageCallback(DebugCallback, 0);
    }
}

internal uint32 CreateShader(const char *vertSource, const char *fragSource)
{
    local_persist const char *HEADER = "#version 430 core";

    const char *vertSources[] = {
        HEADER,
        vertSource
    };

    uint32 vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 2, vertSources, 0);
    glCompileShader(vert);

    int32 status = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(vert, 512, 0, infoLog);
        printf("Vertex shader error: %s\n", infoLog);
        exit(1);
    }

    const char *fragSources[] = {
        HEADER,
        fragSource
    };

    uint32 frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 2, fragSources, 0);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(frag, 512, 0, infoLog);
        printf("Fragment shader error: %s\n", infoLog);
        exit(1);
    }

    uint32 program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, 0, infoLog);
        printf("Program linking error: %s\n", infoLog);
        exit(1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    glValidateProgram(program);

    return program;
}

internal void CreateOpenGLShaders(uint32 *shaders)
{   
    const char *vertShaders[ShaderTypeCount] = {
        R""""(     
            // rect       
            layout (location = 0) in vec2 position;
            layout (location = 1) in vec4 color;
            out vec4 fragColor;
            uniform mat4 projection;
            
            void main()
            {
                fragColor = color;
                gl_Position = projection * vec4(position, 0.0, 1.0);
            }
        )"""",
        R""""(
            // lights
            layout (location = 0) in vec2 position;

            out vec2 fragPos;
            out vec2 fragUv;

            uniform mat4 projection;
            
            void main()
            {
                vec2 uvs[4] = vec2[4](vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 0.0), vec2(1.0, 1.0));
                gl_Position = projection * vec4(position, 0.0, 1.0);
                fragUv = uvs[gl_VertexID];
                fragPos = position;
            }
        )"""",
        R""""(
            // final
            out vec2 fragUv;

            void main()
            {
                vec4 vertices[] = vec4[](vec4(0, 0, 0, 1),
                                        vec4(0, 1, 0, 1),
                                        vec4(1, 0, 0, 1),
                                        vec4(1, 1, 0, 1));
                fragUv = vertices[gl_VertexID].xy;
                gl_Position = vec4(vertices[gl_VertexID].xy * 2 - 1, 0.0, 1.0);
            }
        )"""",
    };

    const char *fragShaders[ShaderTypeCount] = {
        R""""(
            // rect
            layout (location = 0) out vec4 outColor;
            layout (location = 1) out vec4 outNormal;
            in vec4 fragColor;

            void main()
            {
                outColor = fragColor;
                outNormal = vec4(0.25, 0.25, 1, 1);
            }
        )"""",
        R""""(
            // lights
            layout (location = 0) out vec4 outColor;    

            in vec2 fragPos;
            in vec2 fragUv;

            struct light
            {
                vec2 position;
                float minAngle;
                float maxAngle;
                vec4 tint;
                float intensity;
            };

            layout (std140, binding = 0) buffer light_buffer
            {
                light lights[];
            };

            uniform sampler2D normalTexture;
            uniform vec2 resolution;

            #define PI 3.1415926538

            void main()
            {
                light l = lights[0];
                vec2 quadSpace = (2.0 * fragUv) - vec2(1.0);
                float dist = distance(quadSpace, vec2(0));
                float radialFalloff = pow(clamp(1.0 - dist, 0.0, 1.0), 2.0);

                float angle = atan(quadSpace.y, quadSpace.x);
                if (angle < 0) angle += 2*PI;
                float middleAngle = (l.maxAngle + l.minAngle) / 2;
                float angularFalloff = 0.0;
                if (angle <= middleAngle) angularFalloff = smoothstep(l.minAngle, l.maxAngle, angle);
                else if (angle > middleAngle) angularFalloff = smoothstep(l.maxAngle, l.minAngle, angle);

                vec2 uv = gl_FragCoord.xy / resolution;
                vec4 normal = texture(normalTexture, uv);
                vec2 diff = fragPos - l.position;
                vec2 normDiff = normalize(diff);
                float normalFalloff = clamp(dot(normDiff, normal.xy), 0.0, 1.0);

                float finalIntensity = l.intensity * radialFalloff * angularFalloff * normalFalloff;
                vec4 color = l.tint * finalIntensity;

                outColor = color;
            }
        )"""",
        R""""(
            // final
            layout (location = 0) out vec4 outColor;

            in vec2 fragUv;

            uniform sampler2D colorTexture;
            uniform sampler2D lightTexture;

            uniform float volumetricIntensity;

            void main()
            {
                vec4 baseColor = texture(colorTexture, fragUv);
                vec4 lightColor = texture(lightTexture, fragUv);
                vec4 shadedColor = baseColor * lightColor;
                shadedColor += lightColor * volumetricIntensity;
                outColor = shadedColor;
            }
        )"""",
    };

    for (uint32 shaderIndex = 0; shaderIndex < ShaderTypeCount; shaderIndex++)
    {
        shaders[shaderIndex] = CreateShader(vertShaders[shaderIndex], fragShaders[shaderIndex]);
    }
}