#pragma once
#include "libmath/Transform2D.h"
#include "libplatform/OpenGL.h"
#include <SFML/Window/Event.hpp>

class GearsShaderProgram;

struct Gear
{
	math::Transform2D transform;
	ps::VertexBufferObject vbo;
	size_t vertexCount = 0;
};

class GearsScene
{
public:
	GearsScene();
	~GearsScene();

	void Update(float elapsedSeconds);
	void Draw(GearsShaderProgram& program) const;
	bool OnMousePressed(const sf::Event::MouseButtonEvent& event);
	bool OnMouseReleased(const sf::Event::MouseButtonEvent& event);

private:
	ps::VertexArrayObject m_vao;
	Gear m_gear1;
	Gear m_gear2;
	bool m_rotationEnabled = true;
};
