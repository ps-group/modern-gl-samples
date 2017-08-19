#include "AssetLoader.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <glbinding/gl33core/gl.h>
#include <map>

using namespace gl33core;

ps::TextureObject AssetLoader::LoadTexture2D(const fs::path& filepath)
{
	const fs::path abspath = ps::GetAssetAbspath(filepath);

	// Загружаем изображение из файла в оперативную память,
	//  декодируем файловый формат в несжатый bitmap.
	sf::Image image;
	if (!image.loadFromFile(abspath.u8string()))
	{
		throw std::runtime_error("image file has unknown format or cannot be opened: " + filepath.u8string());
	}

	// Создаём ресурс текстуры на стороне видеодрайвера.
	auto texture = ps::CreateTexture();

	// Привязываем ресурс текстуры к текущему контексту.
	glBindTexture(GL_TEXTURE_2D, texture.get());

	// Загружаем текстурные данные на видеокарту, присваивая их
	//  ранее привязанной текстуре.
	const auto size = image.getSize();
	const auto* pixels = image.getPixelsPtr();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y,
		0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Мы не предоставляем mipmap-уроней, поэтому просим видеодрайвер
	//  использовать линейную интерполяцию изображения, если его размеры
	//  не совпадают с размерами текстурируемой фигуры.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

ps::ProgramObject AssetLoader::LoadShaderProgram(const std::vector<std::experimental::filesystem::path>& paths)
{
	// Тип шейдера определяем по расширению файла.
	const std::map<fs::path, GLenum> EXT_MAPPING = {
		{ L".frag", GL_FRAGMENT_SHADER },
		{ L".vert", GL_VERTEX_SHADER },
		{ L".geom", GL_GEOMETRY_SHADER },
	};

	// Компилируем шейдеры и сохраняем в массив shaders
	std::vector<ps::ShaderObject> shaders(paths.size());
	std::transform(paths.begin(), paths.end(), shaders.begin(), [&](const fs::path& filepath) {
		const std::string shaderSource = ps::LoadFileAsString(filepath);
		const GLenum shaderType = EXT_MAPPING.at(filepath.extension());
		return ps::CompileShader(shaderType, shaderSource);
	});

	// Компонуем программу и возвращаем объект программы.
	return ps::LinkProgram(shaders);
}
