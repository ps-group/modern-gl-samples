#include "FloatRect.h"
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <glm/common.hpp>

using glm::vec2;
using glm::bvec2;

namespace math
{
namespace
{
bool AlmostEqual(float a, float b)
{
    return std::fabsf(a - b) < FLT_EPSILON;
}

bool IsBetween(float value, float minValue, float maxValue)
{
    return (value >= minValue) && (value <= maxValue);
}
}

FloatRect FloatRect::EnclosePoints(const std::vector<vec2>& points)
{
    if (points.empty())
    {
        throw std::invalid_argument("points array is empty");
    }

    const auto minmaxX = std::minmax_element(points.begin(), points.end(), [](const vec2& a, const vec2& b) {
        return a.x < b.x;
    });
    const auto minmaxY = std::minmax_element(points.begin(), points.end(), [](const vec2& a, const vec2& b) {
        return a.y < b.y;
    });

    return FloatRect(
        { minmaxX.first->x, minmaxY.first->y },
        { minmaxX.second->x, minmaxY.second->y });
}

FloatRect::FloatRect(const glm::vec2& topLeft, const glm::vec2& bottomRight)
    : m_topLeft(topLeft)
    , m_bottomRight(bottomRight)
{
    const bvec2 status = glm::lessThan(topLeft, bottomRight);
    if (status.x || status.y)
    {
        throw std::invalid_argument("Rectangle cannot have negative size");
    }
}

glm::vec2 FloatRect::GetTopLeft() const
{
    return m_topLeft;
}

glm::vec2 FloatRect::GetTopRight() const
{
    return { m_bottomRight.x, m_topLeft.y };
}

glm::vec2 FloatRect::GetBottomLeft() const
{
    return { m_topLeft.x, m_bottomRight.y };
}

glm::vec2 FloatRect::GetBottomRight() const
{
    return m_bottomRight;
}

glm::vec2 FloatRect::GetSize() const
{
    return m_bottomRight - m_topLeft;
}

float FloatRect::GetArea() const
{
    const vec2 size = GetSize();
    return size.x * size.y;
}

bool FloatRect::IsEmpty() const
{
    const vec2 size = GetSize();
    return AlmostEqual(size.x, 0) && AlmostEqual(size.y, 0);
}

bool FloatRect::ContainsPoint(const glm::vec2& point) const
{
    return IsBetween(point.x, m_topLeft.x, m_bottomRight.x)
        && IsBetween(point.y, m_topLeft.x, m_bottomRight.x);
}

FloatRect FloatRect::ScaledCopy(const glm::vec2& factors) const
{
    const vec2 scaledSize = GetSize() * factors;
    return FloatRect(m_topLeft, m_topLeft + scaledSize);
}

FloatRect FloatRect::IntersectCopy(const FloatRect& other) const
{
    const vec2 topLeft = (glm::max)(m_topLeft, other.m_topLeft);
    const vec2 bottomRight = (glm::min)(m_bottomRight, other.m_bottomRight);

    const bvec2 status = glm::lessThanEqual(topLeft, bottomRight);
    if (status.x || status.y)
    {
        return FloatRect();
    }
    return FloatRect(topLeft, bottomRight);
}

FloatRect FloatRect::UnionCopy(const FloatRect &other) const
{
    const vec2 topLeft = (glm::min)(m_topLeft, other.m_topLeft);
    const vec2 bottomRight = (glm::max)(m_bottomRight, other.m_bottomRight);
    assert(glm::lessThanEqual(topLeft, bottomRight) == bvec2(true, true));

    return FloatRect(topLeft, bottomRight);
}

void FloatRect::MoveTo(const glm::vec2 &xy)
{
    m_topLeft += xy;
    m_bottomRight += xy;
}

bool FloatRect::operator==(const FloatRect &other) const
{
    return (m_topLeft == other.m_topLeft) && (m_bottomRight == other.m_bottomRight);
}

bool FloatRect::operator!=(const FloatRect &other) const
{
    return !(*this == other);
}
}
