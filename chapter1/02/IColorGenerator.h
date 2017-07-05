#pragma once
#include <glm/vec4.hpp>

class IColorGenerator
{
public:
	~IColorGenerator() = default;

	virtual glm::vec4 GenerateColor() = 0;
};
