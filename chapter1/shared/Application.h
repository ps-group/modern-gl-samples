#pragma once
#include "IScene.h"
#include "libplatform/EventLoop.h"
#include <SFML/Window.hpp>

class Application
{
public:
	Application(const sf::VideoMode& videoMode, const char* windowTitle);

	void RunEventLoop(IScene& scene);

private:
	static sf::ContextSettings GetContextSettings();

	sf::Window m_window;
};
