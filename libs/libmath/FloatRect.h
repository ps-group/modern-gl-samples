#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace math
{
class FloatRect
{
public:
    static FloatRect EnclosePoints(const std::vector<glm::vec2>& points);

    FloatRect() = default;
    FloatRect(const glm::vec2& topLeft, const glm::vec2& bottomRight);

    glm::vec2 GetTopLeft() const;
    glm::vec2 GetTopRight() const;
    glm::vec2 GetBottomLeft() const;
    glm::vec2 GetBottomRight() const;
    glm::vec2 GetSize() const;
    float GetArea() const;

    bool IsEmpty() const;
    bool ContainsPoint(const glm::vec2& point) const;

    FloatRect ScaledCopy(const glm::vec2& factors) const;
    FloatRect IntersectCopy(const FloatRect& other) const;
    FloatRect UnionCopy(const FloatRect& other) const;

    void MoveTo(const glm::vec2& xy);

    bool operator==(const FloatRect& other) const;
    bool operator!=(const FloatRect& other) const;

private:
    glm::vec2 m_topLeft;
    glm::vec2 m_bottomRight;
};
}
