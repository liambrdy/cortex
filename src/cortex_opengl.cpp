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
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ARRAY_BUFFER 0x8892
#define GL_FALSE 0
#define GL_FLOAT 0x1406
#define GL_COLOR_BUFFER_BIT 0x00004000

extern "C" void glDrawArrays(GLenum mode, GLint first, GLsizei count);
extern "C" void glClear(GLbitfield mask);
extern "C" void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern "C" void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

typedef void gl_create_vertex_arrays(GLsizei n, GLuint *arrays);
typedef void gl_bind_vertex_array(GLuint array);
typedef void gl_create_buffers(GLsizei n, GLuint *buffers);
typedef void gl_bind_buffer(GLenum target, GLuint buffer);
typedef void gl_buffer_data(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
typedef GLuint gl_create_shader(GLenum shaderType);
typedef GLuint gl_create_program();
typedef void gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void gl_compile_shader(GLuint shader);
typedef void gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
typedef void gl_get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
typedef void gl_get_program_info_log(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void gl_attach_shader(GLuint program, GLuint shader);
typedef void gl_link_program(GLuint program);
typedef void gl_buffer_sub_data(GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
typedef void gl_use_program(GLuint program);
typedef void gl_enable_vertex_attrib_array(GLuint index);
typedef void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);

global gl_create_vertex_arrays *glCreateVertexArrays;
global gl_bind_vertex_array *glBindVertexArray;
global gl_create_buffers *glCreateBuffers;
global gl_bind_buffer *glBindBuffer;
global gl_buffer_data *glBufferData;
global gl_create_shader *glCreateShader;
global gl_create_program *glCreateProgram;
global gl_shader_source *glShaderSource;
global gl_compile_shader *glCompileShader;
global gl_get_shader_iv *glGetShaderiv;
global gl_get_program_iv *glGetProgramiv;
global gl_get_shader_info_log *glGetShaderInfoLog;
global gl_get_program_info_log *glGetProgramInfoLog;
global gl_attach_shader *glAttachShader;
global gl_link_program *glLinkProgram;
global gl_buffer_sub_data *glBufferSubData;
global gl_use_program *glUseProgram;
global gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
global gl_vertex_attrib_pointer *glVertexAttribPointer;

void LoadAllOpenGLFunctions(platform_get_opengl_function *getOpenGLFunction)
{
    if (getOpenGLFunction)
    {
        glCreateVertexArrays = (gl_create_vertex_arrays *)getOpenGLFunction("glCreateVertexArrays");
        glBindVertexArray = (gl_bind_vertex_array *)getOpenGLFunction("glBindVertexArray");
        glCreateBuffers = (gl_create_buffers *)getOpenGLFunction("glCreateBuffers");
        glBindBuffer = (gl_bind_buffer *)getOpenGLFunction("glBindBuffer");
        glBufferData = (gl_buffer_data *)getOpenGLFunction("glBufferData");
        glCreateShader = (gl_create_shader *)getOpenGLFunction("glCreateShader");
        glCreateProgram = (gl_create_program *)getOpenGLFunction("glCreateProgram");
        glShaderSource = (gl_shader_source *)getOpenGLFunction("glShaderSource");
        glCompileShader = (gl_compile_shader *)getOpenGLFunction("glCompileShader");
        glGetShaderiv = (gl_get_shader_iv *)getOpenGLFunction("glGetShaderiv");
        glGetProgramiv = (gl_get_program_iv *)getOpenGLFunction("glGetProgramiv");
        glGetShaderInfoLog = (gl_get_shader_info_log *)getOpenGLFunction("glGetShaderInfoLog");
        glGetProgramInfoLog = (gl_get_program_info_log *)getOpenGLFunction("glGetProgramInfoLog");
        glAttachShader = (gl_attach_shader *)getOpenGLFunction("glAttachShader");
        glLinkProgram = (gl_link_program *)getOpenGLFunction("glLinkProgram");
        glBufferSubData = (gl_buffer_sub_data *)getOpenGLFunction("glBufferSubData");
        glUseProgram = (gl_use_program *)getOpenGLFunction("glUseProgram");
        glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)getOpenGLFunction("glEnableVertexAttribArray");
        glVertexAttribPointer = (gl_vertex_attrib_pointer *)getOpenGLFunction("glVertexAttribPointer");
    }
}

uint32 CreateShader(const char *vertSource, const char *fragSource)
{
    const char *vertSources[] = {
        vertSource
    };

    uint32 vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, vertSources, 0);
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
        fragSource
    };

    uint32 frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, fragSources, 0);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(vert, 512, 0, infoLog);
        printf("Fragment shader error: %s\n", infoLog);
    }

    uint32 program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    return program;
}

void CreateOpenGLShaders(uint32 *shaders)
{
    const char *vertShaders[ShaderTypeCount] = {
        R""""(
            #version 330 core
            
            layout (location = 0) in vec2 position;
            
            void main()
            {
               gl_Position = vec4(position, 0.0, 1.0);
            }
        )"""",
    };

    const char *fragShaders[ShaderTypeCount] = {
        R""""(
            #version 330 core
            
            layout (location = 0) out vec4 outColor;
            
            void main()
            {
               outColor = vec4(0.5, 0.1, 0.1, 1.0);
            }
        )"""",
    };

    for (uint32 shaderIndex = 0; shaderIndex < ShaderTypeCount; shaderIndex++)
    {
        shaders[shaderIndex] = CreateShader(vertShaders[shaderIndex], fragShaders[shaderIndex]);
    }
}