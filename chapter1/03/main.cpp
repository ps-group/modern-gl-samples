#include "Tesselator.h"
#include "libmath/Transform2D.h"
#include "libplatform/EventLoop.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <glbinding/Binding.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <iostream>

using namespace gl33core;

// Этот вершинный шейдер выполняет две задачи:
//  1) передаёт цвет фрагментному шейдеру
//  2) выполняет преобразование вершин модели к мировым координатам
//  3) выполняет ортографическое проецирование вершины,
//     по сути проецируя вершину на плоскость экрана.
static const char kVertexShaderCode[] = R"**(#version 130
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
uniform mat4 u_world_matrix;
void main()
{
    v_color = i_color;
	gl_Position = u_projection_matrix * u_world_matrix * vec4( i_position, 0.0, 1.0 );
}
)**";

// Этот фрагментный шейдер устанавливает фрагменту переданный из
//  вершинного шейдера цвет.
static const char kFragmentShaderCode[] = R"**(#version 130
in vec4 v_color;
void main()
{
    gl_FragColor = v_color;
}
)**";

using DrawCallback = std::function<void()>;

// Превращает rgb(255, 0, 128) в vec4{ 1, 0, 0.5, 1 }
glm::vec4 MakeRGB(unsigned red, unsigned green, unsigned blue)
{
	return glm::vec4(float(red) / 255.f, float(green) / 255.f, float(blue) / 255.f, 1);
};

void BindVertexData(const std::vector<Vertex>& verticies, const sf::Shader& shader)
{
	const GLuint programId = shader.getNativeHandle();

	// OpenGL должен получить байтовые смещения полей относительно структуры Vertex.
	const void* colorOffset = reinterpret_cast<void*>(offsetof(Vertex, rgba));
	const void* posOffset = reinterpret_cast<void*>(offsetof(Vertex, xy));
	const size_t stride = sizeof(Vertex);

	// Привязываем атрибут i_color к данным в вершинном буфере.
	const int colorLocation = glGetAttribLocation(programId, "i_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, glm::vec4().length(), GL_FLOAT, GL_FALSE, stride, colorOffset);

	// Привязываем атрибут i_position к данным в вершинном буфере.
	const int posLocation = glGetAttribLocation(programId, "i_position");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, posOffset);

	// Загружаем данные в вершинный буфер.
	glBufferData(GL_ARRAY_BUFFER, stride * verticies.size(), verticies.data(), GL_STATIC_DRAW);
}

void SetProjectionMatrix(const sf::Window& window, const sf::Shader& shader)
{
	// Вычисляем матрицу ортографического проецирования
	const glm::mat4 value = glm::ortho(0.f, float(window.getSize().x), float(window.getSize().y), 0.f);

	// Передаём матрицу как константу в графической программе
	const GLuint programId = shader.getNativeHandle();
	glUniformMatrix4fv(glGetUniformLocation(programId, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(value));
}

void SetWorldMatrix(const glm::mat4& value, const sf::Shader& shader)
{
	// Передаём матрицу как константу в графической программе
	const GLuint programId = shader.getNativeHandle();
	glUniformMatrix4fv(glGetUniformLocation(programId, "u_world_matrix"), 1, GL_FALSE, glm::value_ptr(value));
}

int main()
{
	try
	{
		// Указываем SFML, что контекст должен иметь определённые атрибуты
		//  - версия OpenGL 3.3 Core Profile
		//  - уровень сглаживания: 4
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		settings.attributeFlags = sf::ContextSettings::Core;
		settings.majorVersion = 3;
		settings.minorVersion = 3;

		// Создаём окно SFML
		sf::Window window(
			sf::VideoMode(800, 600), "OpenGL gears (press left mouse button to stop)",
			sf::Style::Default, settings);

		// Инициализируем библиотеку glbinding после создания окна
		glbinding::Binding::initialize();

		// Компилируем вершинный и фрагментный шейдеры в шейдерную программу
		sf::Shader shader;
		if (!shader.loadFromMemory(kVertexShaderCode, kFragmentShaderCode))
		{
			throw std::runtime_error("vertex or fragment shader compilation failed");
		}
		sf::Shader::bind(&shader);

		// Создаём Vertex Buffer Object (VBO) для загрузки данных,
		//  в этот буфер мы запишем параметры вершин для видеокарты.
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Создаём Vertex Array Object (VAO), который хранит связи между данными
		//  в VBO и переменными шейдера.
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Генерируем список вершин треугольников, представляющих шестерню.
		std::vector<Vertex> verticies = Tesselator::TesselateGear(
			100.f, 180.f, 8, 40.f, { 0, 0 }, MakeRGB(204, 153, 51));

		// Выполняем привязку вершинных данных в контексте текущего VAO.
		BindVertexData(verticies, shader);

		// Устанавливаем матрицу ортографического проецирования.
		SetProjectionMatrix(window, shader);

		constexpr float gearRotationPeriod = 6.f;
		const auto gearAngularVelocity = float(2.f * M_PI / gearRotationPeriod);
		math::Transform2D gearTransform;
		gearTransform.MoveBy({ 400, 300 });
		bool rotationEnabled = true;

		glClearColor(0, 0, 0, 0);

		ps::EventLoop loop;
		loop.DoOnEvent(sf::Event::MouseButtonPressed, [&](const sf::Event& event) {
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				rotationEnabled = false;
			}
		});
		loop.DoOnEvent(sf::Event::MouseButtonReleased, [&](const sf::Event& event) {
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				rotationEnabled = true;
			}
		});
		loop.DoOnUpdate([&](float elapsedSec) {
			if (rotationEnabled)
			{
				gearTransform.RotateBy(gearAngularVelocity * elapsedSec);
			}
		});
		loop.DoOnDraw([&](sf::Window&) {
			glClear(GL_COLOR_BUFFER_BIT);
			// На каждом кадре привязываем VAO
			glBindVertexArray(vao);

			SetWorldMatrix(gearTransform.ToMat4(), shader);
			// Рисуем треугольники, используя вершины из полуинтервала [0, verticies.size)
			glDrawArrays(GL_TRIANGLES, 0, verticies.size());

			if (glGetError() != GL_NO_ERROR)
			{
				throw std::runtime_error("OpenGL error!");
			}
		});
		loop.Run(window);

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "fatal exception: " << ex.what() << std::endl;
	}

	return 0;
}
