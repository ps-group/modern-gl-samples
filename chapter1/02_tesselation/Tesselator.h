#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct Vertex
{
    glm::vec2 position;
    glm::vec4 color;
};

using VertexList = std::vector<Vertex>;

class Tesselator
{
public:
    Tesselator() = delete;

    VertexList BuildEllipse(const glm::vec2& center, const glm::vec2& radius);
    VertexList BuildRectangle(const glm::vec2& position, const glm::vec2& size);
    VertexList BuildFivePointStart(const glm::vec2& center, float radius);
    VertexList BuildTriquetra(const glm::vec2& center, float radius);
};
