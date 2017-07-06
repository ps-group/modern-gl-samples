#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct Vertex
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

class Tesselator
{
public:
	// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
	static std::vector<Vertex> TesselateConvex(const std::vector<glm::vec2>& verticies, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

	// Функция делит круг на треугольники,
	//  возвращает массив с вершинами треугольников.
	static std::vector<Vertex> TesselateCircle(float radius, const glm::vec2& center, const glm::vec4& color = glm::vec4(1, 1, 1, 1));

	// Функция делит шестерню (gear) на треугольники,
	//  возвращает массив с вершинами треугольников.
	static std::vector<Vertex> TesselateGear(
		float innerRadius, float outerRadius, unsigned cogCount, float cogDepth,
		const glm::vec2& center, const glm::vec4& color = glm::vec4(1, 1, 1, 1));
};
