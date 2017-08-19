#include "../shared/Application.h"
#include "MainScene.h"
#include "libplatform/DebugTools.h"
#include <iostream>

int main()
{
	try
	{
		// Создаём приложение с окном SFML
		Application app(sf::VideoMode(800, 600), "Textured shapes");

		// Создаём объект сцены.
		MainScene scene;
		ps::SetupDebugOutput();

		// Запускаем цикл событий.
		app.RunEventLoop(scene);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "fatal exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
