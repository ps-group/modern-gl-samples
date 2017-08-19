#include "CommonMath.h"
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

namespace math
{
// См. https://en.wikipedia.org/wiki/Polar_coordinate_system

glm::vec2 euclidean(float radius, float angle)
{
	return { radius * cos(angle), radius * sin(angle) };
}

glm::vec2 polar(const glm::vec2& value)
{
	const float radius = glm::length(value);
	const float angle = atan2(value.y, value.x);

	return { radius, angle };
}
}
