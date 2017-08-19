#pragma once
#include "../shared/IScene.h"
#include "../shared/RenderStates.h"
#include "../shared/ShaderProgram.h"
#include "TexturedShape.h"
#include "libplatform/OpenGL.h"
#include <vector>

class MainScene final : public IScene
{
public:
	MainScene();

	void Update(ps::seconds dt) final;
	void Draw(const sf::Window& window) const final;
	bool OnMousePressed(const sf::Event::MouseButtonEvent& event) final;
	bool OnMouseReleased(const sf::Event::MouseButtonEvent& event) final;

private:
	mutable RenderStates m_states;
	ShaderProgram m_shader;
	std::vector<TexturedShape> m_shapes;
};
