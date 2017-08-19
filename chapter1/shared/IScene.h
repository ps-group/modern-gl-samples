#pragma once
#include "libplatform/type_aliases.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

class IScene
{
public:
	virtual ~IScene() = default;

	virtual void Update(ps::seconds dt) = 0;
	virtual void Draw(const sf::Window& window) const = 0;
	virtual bool OnMousePressed(const sf::Event::MouseButtonEvent&) { return false; }
	virtual bool OnMouseReleased(const sf::Event::MouseButtonEvent&) { return false; }
	virtual bool OnKeyPressed(const sf::Event::KeyEvent&) { return false; }
	virtual bool OnKeyReleased(const sf::Event::KeyEvent&) { return false; }
};
