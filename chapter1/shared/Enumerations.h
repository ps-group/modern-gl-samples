#pragma once
#include <stddef.h>

// Список стандартных Uniform-переменных, используемых 2D рендерером.
enum UniformId : unsigned
{
	// Фаза анимации
	UniformAnimationPhase = 0,
	// Матрица проецирования, mat3 или mat4
	UniformProjectionMatrix,
	// Матрица преобразования из координат объекта в координаты камеры,
	//  mat3 или mat4
	UniformWorldviewMatrix,
	// Текстура, хранящая цвет фигуры.
	UniformColormap,

	// Специальное значение, равное числу констант enum.
	UniformIdCount
};

// Список стандартных атрибутов, используемых 2D рендерером.
enum AttributeId : unsigned
{
	// Позиция вершины (vec2)
	AttributePosition = 0,
	// Текстурные координаты (vec2)
	AttributeTexCoord,
	// Цвет вершины (vec4)
	AttributeColor,
	// Позиция экземпляра объекта (vec2)
	AttributeInstancePosition,

	// Специальное значение, равное числу констант enum.
	AttributeIdCount
};
