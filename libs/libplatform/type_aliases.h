#pragma once
#include <chrono>

namespace ps
{
// Синоним типа временного интервала с плавающей точкой, представляющего секунды.
using seconds = std::chrono::duration<double>;
}
