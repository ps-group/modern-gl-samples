#include "Filesystem.h"
#include <fstream>

#if defined_WIN32
#include <Windows.h>
#include <sdkddkver.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__apple__)
#include <mach-o/dyld.h>
#endif

const size_t FILE_RESERVE_SIZE = 4096;
const size_t MAX_PATH_SIZE = 4096;

fs::path ps::GetExecutablePath()
{
// На Windows используем GetModuleFileNameW и запрашиваем путь
//  в виде UTF-16 строки.
#if defined(_WIN32)
	wchar_t buffer[MAX_PATH_SIZE];
	size_t size = sizeof(buffer);
	size = size_t(::GetModuleFileNameW(nullptr, buffer, DWORD(size)));
	return std::wstring(buffer, size);

// На Linux читаем символическую ссылку из виртуального файла
//  /proc/self/exe и интерпретируем как UTF-8 строку
#elif defined(__linux__)
	char buffer[MAX_PATH_SIZE];
	size_t size = sizeof(buffer);
	ssize_t result = readlink("/proc/self/exe", buffer, size);
	if (result <= 0)
	{
		throw std::runtime_error("Unexpected error on readlink()");
	}
	size = size_t(result);
	return std::string(buffer, size);

// На OSX используем _NSGetExecutablePath и запрашиваем путь
//  в виде UTF-8 строки.
#elif defined(__apple__)
	char buffer[MAX_PATH_SIZE];
	size_t size = sizeof(buffer);
	uint32_t sizeU32 = uint32_t(size);
	if (_NSGetExecutablePath(buffer, &sizeU32) != 0)
	{
		throw std::runtime_error("Unexpected error on _NSGetExecutablePath()");
	}
	size = size_t(sizeU32);
	return std::string(buffer, size);

#else
#error GetExecutablePath is not implemented for this platform.
#endif
}

fs::path ps::GetAssetAbspath(const std::experimental::filesystem::path& filepath)
{
	if (filepath.is_absolute())
	{
		return filepath;
	}

	const fs::path exeDir = GetExecutablePath().parent_path();
	const fs::path workdir = fs::current_path();
	for (const auto& dir : { exeDir, workdir })
	{
		fs::path abspath = fs::absolute(filepath, dir);
		if (fs::exists(abspath))
		{
			return abspath;
		}
	}
	throw std::runtime_error("Asset not found: " + filepath.u8string());
}

std::string ps::LoadFileAsString(const fs::path& filepath)
{
	const fs::path abspath = GetAssetAbspath(filepath);

	std::ifstream input;
	input.open(filepath.native());
	if (!input.is_open())
	{
		throw std::runtime_error("Cannot open for reading: " + filepath.u8string());
	}

	std::string text;
	text.reserve(FILE_RESERVE_SIZE);
	input.exceptions(std::ios::badbit);

	std::string line;
	while (std::getline(input, line))
	{
		text.append(line);
		text.append("\n");
	}

	return text;
}
