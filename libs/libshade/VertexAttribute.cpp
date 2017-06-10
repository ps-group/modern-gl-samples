#include "VertexAttribute.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

namespace shade
{
VertexAttribute::VertexAttribute(int location)
    : m_location(location)
{
}

VertexAttribute::operator bool() const
{
    return (m_location != -1);
}

void VertexAttribute::Enable() const
{
    if (m_location != -1)
    {
        glEnableVertexAttribArray(GLuint(m_location));
    }
}

void VertexAttribute::Disable() const
{
    if (m_location != -1)
    {
        glDisableVertexAttribArray(GLuint(m_location));
    }
}

void VertexAttribute::SetDivisor(unsigned divisor) const
{
    if (m_location != -1)
    {
        glVertexAttribDivisor(GLuint(m_location), divisor);
    }
}

void VertexAttribute::SetFloatsOffset(size_t offset, size_t stride, unsigned numComponents) const
{
    if (m_location != -1)
    {
        const GLboolean normalize = GL_FALSE;
        glVertexAttribPointer(GLuint(m_location), GLint(numComponents),
                              GL_FLOAT, normalize, GLsizei(stride),
                              reinterpret_cast<const void *>(offset));
    }
}

void VertexAttribute::SetClampedFloatsOffset(size_t offset, size_t stride, unsigned numComponents) const
{
    if (m_location != -1)
    {
        const GLboolean normalize = GL_TRUE;
        glVertexAttribPointer(GLuint(m_location), GLint(numComponents),
                              GL_FLOAT, normalize, GLsizei(stride),
                              reinterpret_cast<const void *>(offset));
    }
}

void VertexAttribute::SetBytesOffset(size_t offset, size_t stride, unsigned numComponents) const
{
    if (m_location != -1)
    {
        glVertexAttribIPointer(GLuint(m_location), GLint(numComponents),
                               GL_UNSIGNED_BYTE, GLsizei(stride),
                               reinterpret_cast<const void *>(offset));
    }
}
}
