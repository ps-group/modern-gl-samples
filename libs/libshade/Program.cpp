#include "Program.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

namespace shade
{
namespace
{
std::string ReadShaderInfoLog(GLuint id)
{
    // При неудаче есть лог ошибок, который мы соберём
    // и в первую очередь надо узнать длину лога.
    GLint logLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    // Зная длину, выделяем строку нужного размера и копируем в неё лог
    std::string log(logLength, '\0');
    GLsizei ignored = 0;
    glGetShaderInfoLog(id, log.size(), &ignored, log.data());

    return log;
}

std::string ReadProgramInfoLog(GLuint id)
{
    // При неудаче есть лог ошибок, который мы соберём
    // и в первую очередь надо узнать длину лога.
    GLint logLength = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    // Зная длину, выделяем строку нужного размера и копируем в неё лог
    std::string log(logLength, '\0');
    GLsizei ignored = 0;
    glGetShaderInfoLog(id, log.size(), &ignored, log.data());

    return log;
}

void CheckShaderStatus(GLuint id)
{
    GLboolean ok = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE)
    {
        // Бросаем исключение, прикрепив к нему лог
        const std::string log = ReadShaderInfoLog(id);
        throw std::runtime_error("shader compilation failed: " + log);
    }
}

void CheckProgramStatus(GLuint id)
{
    GLboolean status = 0;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        // Бросаем исключение, прикрепив к нему лог
        const auto log = ReadProgramInfoLog(id);
        throw std::runtime_error(log);
    }
}
} // anonymous namespace

Program::~Program()
{
    FreeShaders();
    if (m_programId)
    {
        glDeleteProgram(m_programId);
    }
}

void Program::SetAttributes(const AttributeBinding& attributes)
{
    m_attributes = attributes;
}

void Program::SetUniforms(const UniformBinding& uniforms)
{
    m_uniforms = uniforms;
}

void Program::CompileShader(gl::GLenum type, std::string_view source)
{
    // Выделяем ресурс шейдера
    GLuint id = glCreateShader(type);
    try
    {
        // Передаём исходный код шейдера видеодрайверу
        const int length = static_cast<int>(source.length());
        const char* sourceLine = source.data();
        glShaderSource(id, 1, (const GLchar**)&sourceLine, &length);

        // Просим видеодрайвер скомпилировать шейдер и проверяем статус
        glCompileShader(id);
        CheckShaderStatus(id);

        // Сохраняем шейдер для дальнейшей компоновки
        m_shaders.push_back(id);
    }
    catch (...)
    {
        // Освобождаем ресурс и перебрасываем исключение
        glDeleteShader(id);
        throw;
    }
}

void Program::Link()
{
    // Запрашиваем у видеодрайера новый объект.
    GLuint id = glCreateProgram();
    try
    {
        // Прикрепляем ранее скомпилированные шейдеры.
        for (const gl::GLuint shader : m_shaders)
        {
            glAttachShader(id, shader);
        }

        // Просим видеодрайвер выполнить компоновку и проверяем статус.
        glLinkProgram(id);
        CheckProgramStatus(id);

        // Запрашиваем у видеодрайвера адреса атрибутов вершин.
        for (const auto& pair : m_attributes)
        {
            GLint location = glGetAttribLocation(id, pair.second.data());
            m_attributeLocations.emplace(pair.first, VertexAttribute(location));
        }

        // Запрашиваем у видеодрайвера адреса uniform переменных
        for (const auto& pair : m_uniforms)
        {
            GLint location = glGetUniformLocation(id, pair.second.data());
            m_uniformLocations.emplace(pair.first, UniformVariable(location));
        }
    }
    catch (...)
    {
        // Освобождаем ресурс и перебрасываем исключение
        glDeleteProgram(id);
        throw;
    }

    m_programId = id;
    FreeShaders();
}

void Program::Validate() const
{
    glValidateProgram(m_programId);
    CheckProgramStatus(m_programId);
}

void Program::Use() const
{
    glUseProgram(m_programId);
}

UniformVariable Program::GetUniform(UniformId id) const
{
    auto it = m_uniformLocations.find(id);
    if (it != m_uniformLocations.end())
    {
        return it->second;
    }
    throw std::runtime_error("Shader has not attribute " + std::to_string(static_cast<int>(id)));
}

VertexAttribute Program::GetAttribute(AttributeId id) const
{
    auto it = m_attributeLocations.find(id);
    if (it != m_attributeLocations.end())
    {
        return it->second;
    }
    throw std::runtime_error("Shader has not uniform " + std::to_string(static_cast<int>(id)));
}

void Program::FreeShaders()
{
    for (unsigned shaderId : m_shaders)
    {
        glDetachShader(m_programId, shaderId);
        glDeleteShader(shaderId);
    }
    m_shaders.clear();
}

} // namespace shade
