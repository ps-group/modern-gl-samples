#pragma once
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace math
{
// Преобразует координаты из локальных в мировые в следующем порядке:
//  - сначала вершины масштабируются
//    например, единичный круг может превратиться в эллипс
//  - затем поворачиваются
//    т.е. фигуры ориентируются на плоскости
//  - затем переносятся
//    т.е. задаётся положение тела
// изменив порядок, мы изменили бы значение трансформаций.
struct Transform2D
{
public:
	// Позиция фигуры относительно центра мира.
	glm::vec2 position{ 0, 0 };
	// Угол собственного поворота фигуры в радианах.
	float orientation{ 0 };
	// Множители размера фигуры по двум осям.
	glm::vec2 size{ 1, 1 };

	void RotateBy(float radians);
	void ScaleBy(const glm::vec2& scale);
	void ScaleBy(const float scale);
	void MoveBy(const glm::vec2& distance);

	glm::mat3 ToMat3() const;

	glm::mat4 ToMat4() const;
};
}
