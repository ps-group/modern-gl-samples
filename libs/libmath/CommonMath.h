#include <cmath>
#include <glm/fwd.hpp>

// Исправляем особенность Visual C++:
//  по умолчанию константы M_PI и M_E не экспортируются.
#if !defined(M_PI)
constexpr long double M_PI = 3.14159265358979323846;
constexpr long double M_E = 2.71828182845904523536;
#endif

namespace math
{
// Переводит полярные координаты {radius, angle} в декартовы.
// Угол задаётся в радианах.
glm::vec2 euclidean(float radius, float angle);

// Переводит декартовы координаты в полярные.
// Возвращает вектор { радиус, угол }
glm::vec2 polar(const glm::vec2& value);
}
