#include "Transform2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace math
{
void Transform2D::RotateBy(float radians)
{
	this->orientation += radians;
}

void Transform2D::ScaleBy(const glm::vec2& scale)
{
	this->size *= scale;
}

void Transform2D::ScaleBy(const float scale)
{
	this->size *= scale;
}

void Transform2D::MoveBy(const glm::vec2& distance)
{
	this->position += distance;
}

// Метод использует расширение GLM_GTX_matrix_transform_2d
// См. https://glm.g-truc.net/0.9.9/api/a00209.html
glm::mat3 Transform2D::ToMat3() const
{
	glm::mat3 mat;
	mat = glm::translate(mat, position);
	mat = glm::rotate(mat, orientation);
	mat = glm::scale(mat, size);

	return mat;
}

// Метод использует расширения GLM_GTC_matrix_transform
// См. https://glm.g-truc.net/0.9.9/api/a00169.html
glm::mat4 Transform2D::ToMat4() const
{
	glm::mat4 mat;
	mat = glm::translate(mat, { position.x, position.y, 0 });
	mat = glm::rotate(mat, orientation, glm::vec3(0, 0, 1));
	mat = glm::scale(mat, { size.x, size.y, 1 });

	return mat;
}
}
