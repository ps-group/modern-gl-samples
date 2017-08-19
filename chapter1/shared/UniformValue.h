#pragma once
#include "Enumerations.h"
#include <glbinding/gl/enum.h>
#include <glm/glm.hpp>
#include <variant>

using UniformValue = std::variant<
	gl::GLenum,
	int,
	float,
	glm::vec2,
	glm::vec3,
	glm::vec4,
	glm::mat3,
	glm::mat4>;
