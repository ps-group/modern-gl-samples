#pragma once
#include <stddef.h>

namespace shade
{
// Класс инкапсулирует работу с атрибутами вершин.
// Атрибут вершины поверхности - это одно из её свойств,
//  например позиция (XYZ) или цвет (RGBA)
// В современном OpenGL программист сам выбирает набор атрибутов.
class VertexAttribute
{
public:
    explicit VertexAttribute(int location);
    explicit operator bool() const;

    void Enable() const;
    void Disable() const;

    // Устанавливает частоту наложения атрибута в решиме "draw instanced"
    //  - 0 (по умолчанию) - атрибут сопутствует каждой вершине
    //  - 1 (по умолчанию) - атрибут меняется только при смене экземпляра
    void SetDivisor(unsigned divisor) const;

    // Устанавливает смещение атрибута типа `float[numComponents]`
    void SetFloatsOffset(size_t offset, size_t stride, unsigned numComponents) const;

    // Устанавливает смещение атрибута типа `float[numComponents]`
    //  атрибуты будут автоматически приводиться к диапазону [0..1]
    void SetClampedFloatsOffset(size_t offset, size_t stride, unsigned numComponents) const;

    // Устанавливает смещение атрибута типа `uint8_t[numComponents]`
    void SetBytesOffset(size_t offset, size_t stride, unsigned numComponents) const;

private:
    int m_location = -1;
};
} // namespace shade
