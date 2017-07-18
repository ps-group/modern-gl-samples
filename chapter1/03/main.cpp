#include "GearsScene.h"
#include "GearsShaderProgram.h"
#include "libmath/Transform2D.h"
#include "libplatform/EventLoop.h"
#include "libplatform/OpenGL.h"
#include <SFML/Window.hpp>
#include <functional>
#include <glbinding/Binding.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace gl33core;

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

		// Создаём объект сцены, содержащей шестерёнку.
		GearsScene scene;

		// Компилируем вершинный и фрагментный шейдеры в шейдерную программу
		GearsShaderProgram program;
		program.Bind();

		// Устанавливаем матрицу ортографического проецирования.
		const glm::mat4 projMat = glm::ortho(0.f, float(window.getSize().x), float(window.getSize().y), 0.f);
		program.SetProjectionMatrix(projMat);

		// Настраиваем и запускаем цикл событий, выполняющий анимацию.
		ps::EventLoop loop;
		loop.DoOnEvent(sf::Event::MouseButtonPressed, [&](const sf::Event& event) {
			scene.OnMousePressed(event.mouseButton);
		});
		loop.DoOnEvent(sf::Event::MouseButtonReleased, [&](const sf::Event& event) {
			scene.OnMouseReleased(event.mouseButton);
		});
		loop.DoOnUpdate([&](float elapsedSeconds) {
			scene.Update(elapsedSeconds);
		});
		loop.DoOnDraw([&](sf::Window&) {
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			scene.Draw(program);

			if (glGetError() != GL_NO_ERROR)
			{
				throw std::runtime_error("OpenGL error!");
			}
		});
		loop.Run(window);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "fatal exception: " << ex.what() << std::endl;
	}

	return 0;
}
