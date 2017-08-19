#pragma once
#include <functional>
#include <glm/vec2.hpp>
#include <vector>

namespace math
{
// Данная функция-алгоритм генерирует массив точек, вызывая переданную функцию-генератор.
std::vector<glm::vec2> GeneratePoints(
	const unsigned pointCount,
	const std::function<glm::vec2(unsigned index)>& generator);
}
