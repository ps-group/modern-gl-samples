#include "ps_opengl_detail.h"
#include <glbinding/gl32core/gl.h>

using namespace gl32core;

namespace ps::detail
{
void DeleteShader(gl::GLuint handle) noexcept
{
    glDeleteShader(handle);
}

void DeleteProgram(gl::GLuint handle) noexcept
{
    glDeleteProgram(handle);
}

void DeleteVBO(gl::GLuint handle) noexcept
{
    glDeleteBuffers(1, &handle);
}

void DeleteVAO(gl::GLuint handle) noexcept
{
    glDeleteVertexArrays(1, &handle);
}

void SetActiveBufferData(gl::GLenum target, gl::GLenum usage, const uint8_t* bytes, size_t byteCount) noexcept
{
    glBufferData(target, byteCount, bytes, usage);
}

}
