#pragma once
#include "../shared/RenderStates.h"
#include "Mesh.h"
#include "libmath/FloatRect.h"

class MeshUtils
{
public:
	MeshUtils() = delete;

	// Вычисляет вершины и индексы прямоугольника,
	//  разбитого на треугольники.
	static MeshDataP2T2 TesselateQuad(const math::FloatRect& rect);

	// Вычисляет вершины и индексы цветка "полярной розы",
	//  разбитой на треугольники, с центром в точке (0;0).
	// Функция координат кривой в полярных координатах:
	//  localRadius = radius * sin(0.5 * petalCount * angle).
	// @param radius - радиус цветка.
	// @param petalCount - число лепестков.
	static MeshDataP2T2 TesselatePolarRose(float radius, unsigned petalCount);

	// Отправляет данные на видеокарту и возвращает готовую сетку примитивов.
	static MeshP2T2 BuildMesh(const MeshDataP2T2& data);

	static void DrawMesh(const MeshP2T2& mesh, RenderStates& states);
};
