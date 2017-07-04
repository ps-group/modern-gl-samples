#include "Program.h"
#include <glbinding/gl32core/gl.h>
#include <iostream>

using namespace gl32core;

namespace shade
{
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
	// Компилируем и сохраняем шейдер для дальнейшей компоновки
	m_shaders.emplace_back(ps::CompileShader(type, source));
}

void Program::Link()
{
	ps::ProgramObject program = ps::LinkProgram(m_shaders);

	// Запрашиваем у видеодрайвера адреса атрибутов вершин.
	for (const auto& pair : m_attributes)
	{
		GLint location = glGetAttribLocation(m_program.get(), pair.second.data());
		m_attributeLocations.emplace(pair.first, VertexAttribute(location));
	}

	// Запрашиваем у видеодрайвера адреса uniform переменных
	for (const auto& pair : m_uniforms)
	{
		GLint location = glGetUniformLocation(m_program.get(), pair.second.data());
		m_uniformLocations.emplace(pair.first, UniformVariable(location));
	}

	m_program = std::move(program);
	m_shaders.clear();
}

void Program::Validate() const
{
	ps::ValidateProgram(m_program, std::cerr);
}

void Program::Use() const
{
	glUseProgram(m_program.get());
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

} // namespace shade
