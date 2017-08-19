#pragma once
#include "Enumerations.h"
#include <glm/glm.hpp>
#include <variant>

using UniformValue = std::variant<
	TextureId,
	float,
	glm::vec2,
	glm::vec3,
	glm::vec4,
	glm::mat3,
	glm::mat4>;
