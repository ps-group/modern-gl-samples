#pragma once
#include "Tesselator.h"
#include "libshade/IProgram.h"
#include <glbinding/gl/types.h>

class ArrayObject
{
public:
    ArrayObject();
    ArrayObject(const ArrayObject&) = delete;
    ArrayObject& operator=(const ArrayObject&) = delete;
    ~ArrayObject();

    void Bind() const;

private:
    gl::GLuint m_id = 0;
};

class VertexBuffer
{
public:
    VertexBuffer();
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    ~VertexBuffer();

    void Bind() const;
    void UploadData(const VertexList& verticies, shade::IProgram& program);

private:
    gl::GLuint m_id = 0;
};
