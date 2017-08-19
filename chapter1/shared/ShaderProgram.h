#pragma once
#include "IShaderProgram.h"
#include "libplatform/OpenGL.h"
#include <string>
#include <unordered_map>
#include <vector>

using UniformInfo = std::pair<UniformId, std::string>;
using AttributeInfo = std::pair<AttributeId, std::string>;

class ShaderProgram : public IShaderProgram
{
public:
	void Reset(ps::ProgramObject program,
		const std::vector<UniformInfo>& uniformNames,
		const std::vector<AttributeInfo>& attributeNames);

	void Bind() const override;

	int GetUniform(UniformId id) const override;

	int GetAttribute(AttributeId id) const override;

private:
	using UniformMap = std::unordered_map<UniformId, int>;
	using AttributeMap = std::unordered_map<AttributeId, int>;

	void InitUniforms(const std::vector<UniformInfo>& uniformNames);
	void InitAttributes(const std::vector<AttributeInfo>& attributeNames);

	ps::ProgramObject m_program;
	UniformMap m_uniforms;
	AttributeMap m_attributes;
};
