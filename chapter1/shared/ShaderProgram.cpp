#include "ShaderProgram.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

void ShaderProgram::Reset(
	ps::ProgramObject program,
	const std::vector<UniformInfo>& uniformNames,
	const std::vector<AttributeInfo>& attributeNames)
{
	m_program = std::move(program);
	InitUniforms(uniformNames);
	InitAttributes(attributeNames);
}

void ShaderProgram::Bind() const
{
	glUseProgram(m_program.get());
}

int ShaderProgram::GetUniform(UniformId id) const
{
	const auto it = m_uniforms.find(id);
	if (it != m_uniforms.end())
	{
		return it->second;
	}

	// Шейдер не предоставляет такой переменной.
	return -1;
}

int ShaderProgram::GetAttribute(AttributeId id) const
{
	const auto it = m_attributes.find(id);
	if (it != m_attributes.end())
	{
		return it->second;
	}

	// Шейдер не предоставляет такой переменной.
	return -1;
}

void ShaderProgram::InitUniforms(const std::vector<UniformInfo>& uniformNames)
{
	m_uniforms.clear();
	for (const auto& info : uniformNames)
	{
		// Запрашиваем у драйвера номер слота uniform-переменной.
		const std::string& name = info.second;
		const int location = glGetUniformLocation(m_program.get(), name.data());
		if (location == -1)
		{
			throw std::runtime_error("Wrong uniform variable name: " + name);
		}

		// Сохраняем имя и номер слота переменной.
		m_uniforms.emplace(info.first, location);
	}
}

void ShaderProgram::InitAttributes(const std::vector<AttributeInfo>& attributeNames)
{
	m_attributes.clear();
	for (const auto& info : attributeNames)
	{
		// Запрашиваем у драйвера номер слота атрибута.
		const std::string& name = info.second;
		const int location = glGetAttribLocation(m_program.get(), name.data());
		if (location == -1)
		{
			throw std::runtime_error("Wrong vertex attribute name: " + name);
		}

		// Сохраняем имя и номер слота атрибута.
		m_attributes.emplace(info.first, location);
	}
}
