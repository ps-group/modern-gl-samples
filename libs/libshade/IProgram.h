#pragma once
#include "UniformVariable.h"
#include "VertexAttribute.h"
#include "Enumerations.h"

namespace shade
{
class IProgram
{
public:
    virtual ~IProgram() = default;

    // Активирует программу для текущего контекста OpenGL
    virtual void Use() const;

    virtual UniformVariable GetUniform(UniformId id) const = 0;
    virtual VertexAttribute GetAttribute(AttributeId id) const = 0;
};
}
