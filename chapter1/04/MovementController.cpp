#include "MovementController.h"
#include <SFML/Window/Keyboard.hpp>
#include <glm/geometric.hpp>
#include <limits>

using Key = sf::Keyboard::Key;

MovementController::MovementController()
{
	// Заносим подлежащие обработке коды клавиш в словарь.
	m_pressedKeys = {
		{ Key::W, false },
		{ Key::A, false },
		{ Key::S, false },
		{ Key::D, false },
		{ Key::Up, false },
		{ Key::Left, false },
		{ Key::Down, false },
		{ Key::Right, false },
	};
}

glm::vec2 MovementController::GetDirection() const
{
	glm::vec2 direction(0, 0);

	// Проверяем нажатые клавиши и выбираем направление.
	// Движение вверх приортитетнее движения вниз,
	//  движение вправо приоритетнее, чем влево.
	if (m_pressedKeys.at(Key::W) || m_pressedKeys.at(Key::Up))
	{
		direction.y -= 1;
	}
	else if (m_pressedKeys.at(Key::S) || m_pressedKeys.at(Key::Down))
	{
		direction.y += 1;
	}
	if (m_pressedKeys.at(Key::D) || m_pressedKeys.at(Key::Right))
	{
		direction.x += 1;
	}
	else if (m_pressedKeys.at(Key::A) || m_pressedKeys.at(Key::Left))
	{
		direction.x -= 1;
	}

	// Нормализуем направление, иначе при движении по диагонали длина
	//  вектора будет больше единицы.
	if (glm::length(direction) > std::numeric_limits<float>::epsilon())
	{
		direction = glm::normalize(direction);
	}

	return direction;
}

bool MovementController::OnKeyPressed(const sf::Event::KeyEvent& event)
{
	return MaybeSetPressed(event.code, true);
}

bool MovementController::OnKeyReleased(const sf::Event::KeyEvent& event)
{
	return MaybeSetPressed(event.code, false);
}

bool MovementController::MaybeSetPressed(sf::Keyboard::Key key, bool pressed)
{
	// Меняем значение и возвращаем true, если код клавишы содержится в словаре.
	auto it = m_pressedKeys.find(key);
	if (it != m_pressedKeys.end())
	{
		it->second = pressed;
		return true;
	}
	return false;
}
