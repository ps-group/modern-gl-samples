#include "CommonMath.h"
#include <glm/vec2.hpp>

namespace math
{

glm::vec2 euclidean(float radius, float angle)
{
	return { radius * cos(angle), radius * sin(angle) };
}
}
