#include <GL/gl.h>

#include "cortex.h"
#include "cortex_platform.h"

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
    }
}

uint32 CreateShader(char *vertSource, char *fragSource)
{
    uint32 vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSource, 0);
    glCompileShader(vert);

    int32 status = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(vert, 512, 0, infoLog);
        printf("Vertex shader error: %s\n", infoLog);
    }

    uint32 frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSource, 0);
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

    
}

void CreateOpenGLShaders(uint32 *shaders)
{
    for (uint32 shaderIndex = 0; shaderIndex > ShaderTypeCount; shaderIndex++)
    {
        shaders[shaderIndex] = glCreateProgram();

        
    }
}