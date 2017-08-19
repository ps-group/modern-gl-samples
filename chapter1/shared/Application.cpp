#include "Application.h"
#include "libplatform/OpenGL.h"
#include <glbinding/Binding.h>
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

Application::Application(const sf::VideoMode& videoMode, const char* windowTitle)
	: m_window(videoMode, windowTitle, sf::Style::Default, GetContextSettings())
{
	// Инициализируем библиотеку glbinding после создания окна
	glbinding::Binding::initialize();
}

void Application::RunEventLoop(IScene& scene)
{
	// Настраиваем и запускаем цикл событий, выполняющий анимацию.
	ps::EventLoop loop;

	loop.DoOnEvent(sf::Event::MouseButtonPressed, [&](const sf::Event& event) {
		scene.OnMousePressed(event.mouseButton);
	});
	loop.DoOnEvent(sf::Event::MouseButtonReleased, [&](const sf::Event& event) {
		scene.OnMouseReleased(event.mouseButton);
	});
	loop.DoOnEvent(sf::Event::KeyPressed, [&](const sf::Event& event) {
		scene.OnKeyPressed(event.key);
	});
	loop.DoOnEvent(sf::Event::KeyReleased, [&](const sf::Event& event) {
		scene.OnKeyReleased(event.key);
	});
	loop.DoOnUpdate([&](ps::seconds dt) {
		scene.Update(dt);
	});
	loop.DoOnDraw([&](sf::Window& window) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		scene.Draw(window);
		ps::ValidateOpenGLError();
	});
	loop.Run(m_window);
}

sf::ContextSettings Application::GetContextSettings()
{
	// Указываем SFML, что контекст должен иметь определённые атрибуты
	//  - версия OpenGL 3.3 Core Profile
	//  - включён режим отладки
	//  - уровень сглаживания: 4
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	settings.attributeFlags = sf::ContextSettings::Core
		| sf::ContextSettings::Debug;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	return settings;
}
