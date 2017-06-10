#pragma once
#include <experimental/filesystem>

// Модуль filesystem впервые появился в Boost
// В C++17 выпущен экспериментальный стандартный модуль filesystem.
// В будущих версиях C++ модуль войдёт в стандарт, при этом
//   интерфейс может слегка измениться в сравнении с experimental.
namespace fs = std::experimental::filesystem;
