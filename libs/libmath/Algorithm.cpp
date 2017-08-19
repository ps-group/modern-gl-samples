#include "Algorithm.h"

std::vector<glm::vec2> math::GeneratePoints(
	const unsigned pointCount,
	const std::function<glm::vec2(unsigned)>& generator)
{
	std::vector<glm::vec2> points(pointCount);
	for (unsigned pi = 0; pi < pointCount; ++pi)
	{
		points[pi] = generator(pi);
	}
	return points;
}
