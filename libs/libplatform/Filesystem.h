#pragma once
#include <experimental/filesystem>
#include <string>

// Модуль filesystem впервые появился в Boost
// В C++17 выпущен экспериментальный стандартный модуль filesystem.
// В будущих версиях C++ модуль войдёт в стандарт, при этом
//   интерфейс может слегка измениться в сравнении с experimental.
namespace fs = std::experimental::filesystem;

namespace ps
{
// Возвращает путь к текущему исполняемому файлу,
//  то есть к .exe на Windows, ELF на Linux, MachO на MacOSX
fs::path GetExecutablePath();

// Возвращет абсолютный путь к ресурсу
//  - если путь уже абсолютный, возвращает его
//  - иначе интерпретирует путь как относительный путь
//    в каталоге исполняемого файла либо в рабочем каталоге,
//    выбирая из двух вариантов первый существующий путь.
//  - если ни один из путей не существует, бросает исключение.
fs::path GetAssetAbspath(const fs::path& filepath);

// Загружает всё содержимое файла в строку.
std::string LoadFileAsString(const fs::path& filepath);
}
