#pragma once
#include "libplatform/OpenGL.h"
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

class GearsShaderProgram
{
public:
	GearsShaderProgram();
	~GearsShaderProgram();

	GearsShaderProgram(const GearsShaderProgram&) = delete;
	GearsShaderProgram& operator=(const GearsShaderProgram&) = delete;

	// Метод выполняет вызов glUseProgram, активирующий шейдерную программу
	//  в текущем контексте.
	void Bind();

	// Метод выполняет привязку interleaved arrays, хранящихся в VBO,
	//  к атрибутам шейдерной программы:
	//  - атрибут i_position получает позицию из поля Vertex::xy
	//  - атрибут i_color получает цвет из поля Vertex::rgba
	// Привязка происходит в текущем контексте VAO.
	void BindAttributes(const ps::VertexBufferObject& vbo);

	// Устанавливает uniform-переменную u_projection_matrix,
	//  выполняющую роль матрицы проецирования из глобальных координат
	//  2D-мира в нормализованное представление координат.
	void SetProjectionMatrix(const glm::mat4& value);

	// Устанавливает uniform-переменную u_world_matrix,
	//  выполняющую роль матрицы аффинного преобразования
	//  из локальных координат объекта в глобальные координаты.
	void SetWorldMatrix(const glm::mat4& value);

private:
	void SetMatrixUniform(const char name[], const glm::mat4& value);

	ps::ProgramObject m_program;
};
