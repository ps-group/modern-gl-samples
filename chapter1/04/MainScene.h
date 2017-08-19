#pragma once
#include "../shared/IScene.h"
#include "../shared/RenderStates.h"
#include "../shared/ShaderProgram.h"
#include "MovementController.h"
#include "TexturedShape.h"
#include "libplatform/OpenGL.h"
#include <vector>

class MainScene final : public IScene
{
public:
	MainScene();

	void Update(ps::seconds dt) final;
	void Draw(const sf::Window& window) const final;
	bool OnKeyPressed(const sf::Event::KeyEvent& event) final;
	bool OnKeyReleased(const sf::Event::KeyEvent& event) final;

private:
	void DrawShape(const TexturedShape& shape) const;

	mutable RenderStates m_states;
	MovementController m_movementController;
	ShaderProgram m_shader;
	TexturedShape m_spaceship;
};
