#pragma once
#include <glm/fwd.hpp>
#include <vector>

namespace shade
{
class UniformVariable
{
public:
    explicit UniformVariable(int location);

    // Отправка скалярных и векторных значений
    void SetValue(int value);
    void SetValue(float value);
    void SetValue(const glm::vec2& value);
    void SetValue(const glm::ivec2& value);
    void SetValue(const glm::vec3& value);
    void SetValue(const glm::vec4& value);
    void SetValue(const glm::mat3& value);
    void SetValue(const glm::mat4& value);

    // Отправка массива матриц.
    void SetValue(const std::vector<glm::mat4>& value);

    // Блокируем случайное использование других типов.
    void SetValue(bool) = delete;
    void SetValue(double value) = delete;
    void SetValue(unsigned value) = delete;
    void SetValue(const void*) = delete;

private:
    int m_location = -1;
};
} // namespace shade
