#pragma once
#include "IProgram.h"
#include "libplatform/ps_opengl.h"
#include <string>
#include <unordered_map>

namespace shade
{
using AttributeBinding = std::vector<std::pair<AttributeId, std::string_view>>;
using UniformBinding = std::vector<std::pair<UniformId, std::string_view>>;

// Класс инкапсулирует работу с шейдерной программой.
// Программа соединяет несколько шейдеров разных типов.
// Шейдер каждого типа позволяет программисту выполнить на
//  определённом этапе рендеринга собственный алгоритм, передав
//  видеодрайверу исходный код на языке GLSL.
class Program : public IProgram
{
public:
    // Объект поддерживает перемещение, но копирование запрещено.
    Program() = default;
    Program(Program&&) = default;
    Program& operator =(Program&&) = default;
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    ~Program();

    void SetAttributes(const AttributeBinding& attributes);
    void SetUniforms(const UniformBinding& uniforms);

    // Компилирует шейдер заданного типа из строки с исходным кодом
    // @param type - это GL_VERTEX_SHADER, GL_FRAGMENT_SHADER или GL_GEOMETRY_SHADER.
    void CompileShader(gl::GLenum type, std::string_view source);

    // Фасад функции glLinkProgram
    // Выполняет компоновку шейдеров в единую программу
    void Link();

    // Валидация - необязательный этап, который может сообщить
    // о проблемах производительности или предупреждениях компилятора GLSL
    void Validate() const;

    // Активирует программу для текущего контекста OpenGL
    void Use() const override;

    UniformVariable GetUniform(UniformId id) const override;
    VertexAttribute GetAttribute(AttributeId id) const override;

private:
    AttributeBinding m_attributes;
    UniformBinding m_uniforms;
    std::unordered_map<UniformId, UniformVariable> m_uniformLocations;
    std::unordered_map<AttributeId, VertexAttribute> m_attributeLocations;
    ps::ProgramObject m_program;
    std::vector<ps::ShaderObject> m_shaders;
};
} // namespace shade
