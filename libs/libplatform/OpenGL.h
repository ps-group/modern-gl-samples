#pragma once
#include "detail/OpenGL_detail.h"
#include <iosfwd>
#include <string_view>
#include <vector>

namespace ps
{
using ShaderObject = detail::GfxObject<detail::DeleteShader>;
using ProgramObject = detail::GfxObject<detail::DeleteProgram>;
using VertexBufferObject = detail::GfxObject<detail::DeleteVBO>;
using VertexArrayObject = detail::GfxObject<detail::DeleteVAO>;

// Фасад функции glCompileShader
// Компилирует шейдер заданного типа из строки с исходным кодом
// @param type - это GL_VERTEX_SHADER, GL_FRAGMENT_SHADER или GL_GEOMETRY_SHADER.
ShaderObject CompileShader(gl::GLenum type, std::string_view source);

// Фасад функции glLinkProgram
// Выполняет компоновку шейдеров в единую программу
ProgramObject LinkProgram(const std::vector<ShaderObject>& shaders);

// Фасад функции glValidateProgram
// Валидация - необязательный этап, который может сообщить
// о проблемах производительности или предупреждениях компилятора GLSL
// Если проблемы есть, они выводятся в поток out.
void ValidateProgram(const ProgramObject& program, std::ostream& out);

// Фасад функции glGenVertexArrays
// Создаёт VAO, хранящий связь буферов данных и атрибутов вершин.
VertexArrayObject CreateVAO();

// Фасад функций glGenBuffers и glBufferData
// Передаёт на видеокарту заданные байты
// @param target - это GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER или GL_UNIFORM_BUFFER
VertexBufferObject CreateStaticVBO(gl::GLenum target, const std::byte* bytes, const size_t byteCount);

// Эта версия функции принимает данные в виде массива. Допустимы следующие типы:
// - T[], массив в стиле C
// - std::vector<T>
// - std::array<T>
// - std::initializer_list<T>, т.е. списки инициализации
template<class ArrayT>
VertexBufferObject CreateStaticVBO(gl::GLenum target, const ArrayT& verticies)
{
	// VertexT - тип элемента массива
	using VertexT = std::decay_t<decltype(verticies[0])>;

	// Превращаем указатель на начало массива в указатель на байты массива.
	const auto bytes = reinterpret_cast<const std::byte*>(std::data(verticies));
	// Вычисляем размер массива в байтах.
	const size_t bytesCount = sizeof(VertexT) * std::size(verticies);

	return CreateStaticVBO(target, bytes, bytesCount);
}
}
