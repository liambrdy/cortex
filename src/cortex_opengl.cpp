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

extern "C" void glDrawArrays(GLenum mode, GLint first, GLsizei count);
extern "C" void glClear(GLbitfield mask);
extern "C" void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern "C" void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
extern "C" void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
extern "C" GLenum glGetError();

#define GL_FUN(name, type_ret, type, ...) \
typedef type_ret type(__VA_ARGS__); \
global type *name;

GL_FUN(glCreateVertexArrays, void, gl_create_vertex_arrays, GLsizei n, GLuint *arrays);
GL_FUN(glBindVertexArray, void, gl_bind_vertex_array, GLuint array);
GL_FUN(glCreateBuffers, void, gl_create_buffers, GLsizei n, GLuint *buffers);
GL_FUN(glBindBuffer, void, gl_bind_buffer, GLenum target, GLuint buffer);
GL_FUN(glBufferData, void, gl_buffer_data, GLenum target, GLsizeiptr size, const void * data, GLenum usage);
GL_FUN(glCreateShader, GLuint, gl_create_shader, GLenum shaderType);
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
GL_FUN(glEnableVertexAttribArray, void, gl_enable_vertex_attrib_array, GLuint index);
GL_FUN(glVertexAttribPointer, void, gl_vertex_attrib_pointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
GL_FUN(glCreateTextures, void, gl_create_textures, GLenum target, GLsizei n, GLuint *textures);
GL_FUN(glBindTexture, void, gl_bind_texture, GLenum target, GLuint texture);
GL_FUN(glCreateFramebuffers, void, gl_create_framebuffers, GLsizei n, GLuint *framebuffers);
GL_FUN(glBindFramebuffer, void, gl_bind_framebuffer, GLenum target, GLuint framebuffer);
//GL_FUN(glTexImage2D, void, gl_tex_image_2d, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);
GL_FUN(glFramebufferTexture, void, gl_framebuffer_texture, GLenum target, GLenum attachment, GLuint texture, GLint level);
GL_FUN(glDrawBuffers, void, gl_draw_buffers, GLsizei n, const GLenum *bufs);
GL_FUN(glBlitFramebuffer, void, gl_blit_framebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

#undef GL_FUN

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
        GL_FUN(glEnableVertexAttribArray, gl_enable_vertex_attrib_array);
        GL_FUN(glVertexAttribPointer, gl_vertex_attrib_pointer);
        GL_FUN(glCreateTextures, gl_create_textures);
        GL_FUN(glBindTexture, gl_bind_texture);
        GL_FUN(glCreateFramebuffers, gl_create_framebuffers);
        GL_FUN(glBindFramebuffer, gl_bind_framebuffer);
        //GL_FUN(glTexImage2D, gl_tex_image_2d);
        GL_FUN(glFramebufferTexture, gl_framebuffer_texture);
        GL_FUN(glDrawBuffers, gl_draw_buffers);
        GL_FUN(glBlitFramebuffer, gl_blit_framebuffer);
#undef GL_FUN
    }
}

internal uint32 CreateShader(const char *vertSource, const char *fragSource)
{
    local_persist const char *HEADER = "#version 330 core";

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
    }

    uint32 program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    return program;
}

internal void CreateOpenGLShaders(uint32 *shaders)
{
    const char *vertShaders[ShaderTypeCount] = {
        R""""(            
            layout (location = 0) in vec2 position;
            layout (location = 1) in vec4 color;
            out vec4 fragColor;
            
            void main()
            {
                fragColor = color;
                gl_Position = vec4(position, 0.0, 1.0);
            }
        )"""",
        R""""(
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
            layout (location = 0) out vec4 outColor;
            layout (location = 1) out vec4 outNormal;
            in vec4 fragColor;

            void main()
            {
                outColor = fragColor;
                outNormal = vec4(0, 1, 0, 1);
            }
        )"""",
        R""""(
            layout (location = 0) out vec4 outColor;

            in vec2 fragUv;

            uniform sampler2D colorTexture;
            uniform sampler2D normalTexture;

            void main()
            {
                outColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )"""",
    };

    for (uint32 shaderIndex = 0; shaderIndex < ShaderTypeCount; shaderIndex++)
    {
        shaders[shaderIndex] = CreateShader(vertShaders[shaderIndex], fragShaders[shaderIndex]);
    }
}