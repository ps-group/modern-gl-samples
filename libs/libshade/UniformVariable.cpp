#include "UniformVariable.h"
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>

using namespace gl33core;

namespace shade
{
UniformVariable::UniformVariable(int location)
    : m_location(location)
{
}

void UniformVariable::SetValue(int value)
{
    if (m_location != -1)
    {
        glUniform1i(m_location, value);
    }
}

void UniformVariable::SetValue(float value)
{
    if (m_location != -1)
    {
        glUniform1f(m_location, value);
    }
}

void UniformVariable::SetValue(const glm::vec2& value)
{
    if (m_location != -1)
    {
        glUniform2fv(m_location, 1, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const glm::ivec2& value)
{
    if (m_location != -1)
    {
        glUniform2iv(m_location, 1, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const glm::vec3& value)
{
    if (m_location != -1)
    {
        glUniform3fv(m_location, 1, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const glm::vec4& value)
{
    if (m_location != -1)
    {
        glUniform4fv(m_location, 1, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const glm::mat3& value)
{
    if (m_location != -1)
    {
        glUniformMatrix3fv(m_location, 1, false, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const glm::mat4& value)
{
    if (m_location != -1)
    {
        glUniformMatrix4fv(m_location, 1, false, glm::value_ptr(value));
    }
}

void UniformVariable::SetValue(const std::vector<glm::mat4>& value)
{
    if ((m_location != -1) && !value.empty())
    {
        const auto count = GLsizei(value.size());
        glUniformMatrix4fv(m_location, count, false, glm::value_ptr(value[0]));
    }
}
}
