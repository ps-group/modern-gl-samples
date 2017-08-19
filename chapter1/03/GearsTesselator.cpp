#include "GearsTesselator.h"
#include "libmath/Algorithm.h"
#include "libmath/CommonMath.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <glm/glm.hpp>
#include <numeric>
#include <type_traits>

// Получает локальный внешний радиус шестерни при заданном угле сечения.
float GetGearLocalRadius(float outerRadius, unsigned cogCount, float cogDepth, float angle)
{
	// Вычисляем длину одного сектора (включающего зуб и промежуток).
	const auto sectorLength = static_cast<float>(2 * M_PI / cogCount);

	const float minRadius = outerRadius - cogDepth;

	// Вычисляем "фазу" от 0 до 1, определяющую положение точки в секторе.
	const float phase = fmodf(angle, sectorLength) / sectorLength;

	// Длина зуба составляет 60% длины сектора, из них 10% составляет пологая стенка зуба.
	constexpr float cogRatio = 0.6f;
	constexpr float cogWallRatio = 0.1f;

	// В первом интервале радиус вычисляется путём интерполяции
	//  между минимальным и максимальным внешними радиусами
	if (phase < cogWallRatio)
	{
		const float factor = phase / cogWallRatio;
		return glm::mix(minRadius, outerRadius, factor);
	}
	// Во втором интервале радиус максимален (это кромка зуба).
	if (phase < cogRatio - cogWallRatio)
	{
		return outerRadius;
	}
	// В третьем интервале радиус вычисляется путём интерполяции
	//  между минимальным и максимальным внешними радиусами
	if (phase < cogRatio)
	{
		const float factor = (cogRatio - phase) / cogWallRatio;
		return glm::mix(minRadius, outerRadius, factor);
	}
	// Во четвёртом интервале радиус минимален (это полость между зубьями).
	return minRadius;
}

std::vector<Vertex> GearsTesselator::TesselateGear(
	float innerRadius, float outerRadius, unsigned cogCount, float cogDepth,
	const glm::vec2& center, const glm::vec4& color)
{
	assert((innerRadius > 0) && (outerRadius > 0) && (cogDepth > 0));
	assert(innerRadius + cogDepth < outerRadius);

	// Шестерня будет разделена на 360 участков.
	constexpr unsigned divisionLevel = 360;

	// Точки внутренней оболочки, имеющей вид окружности.
	std::vector<glm::vec2> innerPoints = math::GeneratePoints(divisionLevel, [&](unsigned index) {
		const auto angle = static_cast<float>(2.f * M_PI * index / divisionLevel);
		return center + math::euclidean(innerRadius, angle);
	});

	// Точки внешней оболочки, содержащей зубья.
	std::vector<glm::vec2> outerPoints = math::GeneratePoints(divisionLevel, [&](unsigned index) {
		const auto angle = static_cast<float>(2.f * M_PI * index / divisionLevel);
		const float localRadius = GetGearLocalRadius(outerRadius, cogCount, cogDepth, angle);

		return center + math::euclidean(localRadius, angle);
	});

	std::vector<Vertex> verticies;
	verticies.reserve(6u * divisionLevel);
	for (size_t di = 0; di < divisionLevel; ++di)
	{
		// Мы выбираем по 2 точки с внешней и внутренней оболочек
		//  и создаём 6 вершин для 2 треугольников.
		const size_t nextdi = (di + 1) % divisionLevel;

		verticies.push_back({ outerPoints.at(di), color });
		verticies.push_back({ innerPoints.at(di), color });
		verticies.push_back({ outerPoints.at(nextdi), color });

		verticies.push_back({ outerPoints.at(nextdi), color });
		verticies.push_back({ innerPoints.at(di), color });
		verticies.push_back({ innerPoints.at(nextdi), color });
	}

	return verticies;
}
