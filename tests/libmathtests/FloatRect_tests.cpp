#include "catch.hpp"

#include "libmath/FloatRect.h"

using math::FloatRect;

TEST_CASE("Calculates all corner points", "[FloatRect]" )
{
    glm::vec2 expectedTopLeft = { 100, 200 };
    glm::vec2 expectedTopRight = { 300, 200 };
    glm::vec2 expectedBottomLeft = { 100, 250 };
    glm::vec2 expectedBottomRight = { 300, 250 };
    glm::vec2 expectedSize = { 200, 50 };

    const FloatRect rect(expectedTopLeft, expectedBottomRight);
    REQUIRE(rect.GetTopLeft() == expectedTopLeft);
    REQUIRE(rect.GetTopRight() == expectedTopRight);
    REQUIRE(rect.GetBottomLeft() == expectedBottomLeft);
    REQUIRE(rect.GetBottomRight() == expectedBottomRight);
    REQUIRE(rect.GetSize() == expectedSize);
}
