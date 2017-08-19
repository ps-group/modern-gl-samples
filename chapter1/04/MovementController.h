#pragma once
#include <SFML/Window/Event.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>

class MovementController
{
public:
	MovementController();

	// Возвращает вектор (0, 0), если управляющие клавиши не нажаты,
	//  иначе возвращает нормализованный вектор направления движения.
	glm::vec2 GetDirection() const;

	// Возвращает true, если событие обработано.
	bool OnKeyPressed(const sf::Event::KeyEvent& event);

	// Возвращает true, если событие обработано.
	bool OnKeyReleased(const sf::Event::KeyEvent& event);

private:
	bool MaybeSetPressed(sf::Keyboard::Key key, bool pressed);

	std::unordered_map<sf::Keyboard::Key, bool> m_pressedKeys;
};
