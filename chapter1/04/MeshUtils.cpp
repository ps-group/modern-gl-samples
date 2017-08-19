#include "MeshUtils.h"
#include "libmath/Algorithm.h"
#include "libmath/CommonMath.h"

using namespace gl;
using glm::vec2;

namespace
{
// Координаты лежат в прямоугольнике bounds,
//  а текстурные коодинаты лежат в диапазоне 0..1
// Функция вычисляет текстурные координаты вершины в диапазоне 0..1
VertexP2T2 MakeVertex(const vec2& point, const math::FloatRect& bounds)
{
	assert(bounds.ContainsPoint(point));

	VertexP2T2 v;
	v.position = point;
	// Приводим позицию к диапазону 0..1, это и будут текстурные координаты.
	v.texCoord = (point - bounds.GetTopLeft()) / bounds.GetSize();

	return v;
}
}

MeshDataP2T2
MeshUtils::TesselateQuad(const math::FloatRect& rect)
{
	// Прямоугольник делим на два треугольника по диагонали.
	MeshDataP2T2 data;
	data.primitive = GL_TRIANGLES;
	data.verticies = {
		VertexP2T2{ rect.GetTopLeft(), vec2(0, 0) },
		VertexP2T2{ rect.GetTopRight(), vec2(1, 0) },
		VertexP2T2{ rect.GetBottomLeft(), vec2(0, 1) },
		VertexP2T2{ rect.GetBottomRight(), vec2(1, 1) },
	};
	data.indicies = { 0, 1, 2, 1, 3, 2 };

	return data;
}

MeshDataP2T2 MeshUtils::TesselatePolarRose(float radius, unsigned petalCount)
{
	// Число точек равно длине окружности для максимальной детализации.
	const auto pointCount = static_cast<unsigned>(2 * M_PI * radius);
	// Вычисляем точки кривой, замыкающей фигуру.
	const auto outerPoints = math::GeneratePoints(pointCount, [&](unsigned pi) {
		const auto angle = float(2.f * M_PI * pi / pointCount);
		const auto localRadius = float(radius * sin(0.5 * petalCount * angle));

		return math::euclidean(localRadius, angle);
	});

	MeshDataP2T2 data;
	data.primitive = GL_TRIANGLES;

	// Преобразуем массив точек кривой в массив вершин.
	math::FloatRect bounds(vec2(-radius), vec2(radius));
	for (const auto& point : outerPoints)
	{
		data.verticies.push_back(MakeVertex(point, bounds));
	}

	// Точка (0;0) является центром и имеет индекс, равный pointCount.
	data.verticies.push_back(MakeVertex(vec2(0, 0), bounds));

	for (size_t pi = 0; pi < pointCount; ++pi)
	{
		// Точка на кривой.
		data.indicies.push_back(pi);
		// Следующая точка на кривой.
		data.indicies.push_back((pi + 1) % pointCount);
		// Центр треугольника.
		data.indicies.push_back(pointCount);
	}

	return data;
}

MeshP2T2 MeshUtils::BuildMesh(const MeshDataP2T2& data)
{
	MeshP2T2 mesh;
	mesh.primitive = data.primitive;
	mesh.indexCount = data.indicies.size();
	mesh.vertexCount = data.verticies.size();
	mesh.verticies = ps::CreateStaticVBO(GL_ARRAY_BUFFER, data.verticies);
	mesh.indicies = ps::CreateStaticVBO(GL_ELEMENT_ARRAY_BUFFER, data.indicies);

	return mesh;
}

void MeshUtils::DrawMesh(const MeshP2T2& mesh, RenderStates& states)
{
	// Привязываем буфер вершин и буфер индексов вершин.
	states.BindIndiciesBuffer(mesh.indicies);
	states.BindVerticiesBuffer(mesh.verticies);

	// Привязываем атрибуты position и texCoord к входным
	//  переменным вершинного шейдера.
	states.ActivateAttributes({ AttributePosition, AttributeTexCoord });
	states.SetAttributeOffset(
		AttributePosition, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexP2T2), offsetof(VertexP2T2, position));
	states.SetAttributeOffset(
		AttributeTexCoord, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexP2T2), offsetof(VertexP2T2, texCoord));

	// Запускаем проход рисования
	states.DrawElements(
		mesh.primitive, 0, unsigned(mesh.vertexCount),
		mesh.indexCount, GL_UNSIGNED_INT);
}
