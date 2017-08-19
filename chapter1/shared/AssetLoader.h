#pragma once
#include "ShaderProgram.h"
#include "libplatform/Filesystem.h"
#include "libplatform/OpenGL.h"

class AssetLoader
{
public:
	static ps::TextureObject LoadTexture2D(const fs::path& filepath);

	static ps::ProgramObject LoadShaderProgram(const std::vector<fs::path>& paths);
};
