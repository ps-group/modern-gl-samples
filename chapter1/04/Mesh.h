#pragma once

#include "libplatform/OpenGL.h"
#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>
#include <glm/vec2.hpp>
#include <vector>

// Вершина с двумерной позицией и 2D координатами текстуры.
struct VertexP2T2
{
	glm::vec2 position;
	glm::vec2 texCoord;
};

// Сетка графических примитивов
// Вершины могут переиспользоваться благодаря массиву индексов.
struct MeshDataP2T2
{
	// Тип примитива: обычно GL_POINTS, GL_LINES или GL_TRIANGLES.
	gl::GLenum primitive = gl::GL_TRIANGLES;
	// Вершины, составляющие примитивы сетки.
	std::vector<VertexP2T2> verticies;
	// Список индексов, в котором последовательно перечислены индексы
	//  каждой вершины каждого примитива.
	std::vector<uint32_t> indicies;
};

struct MeshP2T2
{
	// Тип примитива: обычно GL_POINTS, GL_LINES или GL_TRIANGLES.
	gl::GLenum primitive = gl::GL_TRIANGLES;
	// Число индексов для отрисовки.
	size_t indexCount = 0;
	// Число вершин для отрисовки.
	size_t vertexCount = 0;
	// Буфер, хранящий атрибуты вершин.
	ps::VertexBufferObject verticies;
	// Буфер, хранящий индесы вершин.
	ps::VertexBufferObject indicies;
};
