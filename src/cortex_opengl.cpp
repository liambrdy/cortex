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

global gl_create_vertex_arrays *glCreateVertexArrays;
global gl_bind_vertex_array *glBindVertexArray;
global gl_create_buffers *glCreateBuffers;
global gl_bind_buffer *glBindBuffer;
global gl_buffer_data *glBufferData;
global gl_create_shader *glCreateShader;
global gl_create_program *glCreateProgram;

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
    }
}

void CreateOpenGLShaders(uint32 *shaders)
{
    for (uint32 shaderIndex = 0; shaderIndex > ShaderTypeCount; shaderIndex++)
    {
        shaders[shaderIndex] = glCreateProgram();

        
    }
}