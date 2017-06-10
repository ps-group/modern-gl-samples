#include "VertexBuffer.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

ArrayObject::ArrayObject()
{
    glGenVertexArrays(1, &m_id);
}

ArrayObject::~ArrayObject()
{
    glDeleteVertexArrays(1, &m_id);
}

void ArrayObject::Bind() const
{
    glBindVertexArray(m_id);
}

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::UploadData(const VertexList &verticies, shade::IProgram &program)
{
    auto colorAttr = program.GetAttribute(shade::AttributeId::COLOR);
    colorAttr.Enable();
    colorAttr.SetFloatsOffset(offsetof(Vertex, color), sizeof(Vertex), 4);

    auto positionAttr = program.GetAttribute(shade::AttributeId::POSITION);
    positionAttr.Enable();
    positionAttr.SetFloatsOffset(offsetof(Vertex, position), sizeof(Vertex), 2);

    const auto byteCount = static_cast<GLsizeiptr>(sizeof(Vertex) * verticies.size());
    glBufferData(GL_ARRAY_BUFFER, byteCount, verticies.data(), GL_STATIC_DRAW);
}
