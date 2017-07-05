#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>

using DrawCallback = std::function<void(sf::RenderTarget& target)>;

// Исполняет цикл рисования, завершается при закрытии окна.
void EnterRenderLoop(sf::RenderWindow& window, const DrawCallback& draw)
{
	while (window.isOpen())
	{
		// Обработка событий
		sf::Event event = { sf::Event::Closed, 0 };
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		// Рисование состояния
		window.clear();
		draw(window);
		// Вывод кадра на экран
		window.display();
	}
}

void DrawShapes(sf::RenderTarget& target)
{
	sf::CircleShape circle(50);
	circle.setPosition(300, 230);
	circle.setFillColor(sf::Color(100, 250, 50));
	target.draw(circle);

	sf::ConvexShape convex;
	convex.setPosition(100, 200);
	convex.setPointCount(5);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(150, 10));
	convex.setPoint(2, sf::Vector2f(120, 90));
	convex.setPoint(3, sf::Vector2f(30, 100));
	convex.setPoint(4, sf::Vector2f(0, 50));
	target.draw(convex);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(
		sf::VideoMode(800, 600), "SFML shapes",
		sf::Style::Default, settings);

	EnterRenderLoop(window, DrawShapes);

	return 0;
}
