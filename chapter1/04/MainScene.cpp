#include "MainScene.h"
#include "../shared/AssetLoader.h"
#include "MeshUtils.h"
#include <glm/gtc/matrix_transform.hpp>

namespace
{
constexpr float SPACESHIP_VELOCITY = 100;

MeshP2T2 MakeSpriteMesh()
{
	const math::FloatRect rect({ -0.5f, -0.5f }, { 0.5f, 0.5f });
	return MeshUtils::BuildMesh(MeshUtils::TesselateQuad(rect));
}
}

MainScene::MainScene()
{
	m_spaceship.texture = AssetLoader::LoadTexture2D("res/04/spaceship.png");
	m_spaceship.mesh = MakeSpriteMesh();
	m_spaceship.transform.ScaleBy({ 73, 50 });
	m_spaceship.transform.MoveBy({ 200, 250 });

	auto obj = AssetLoader::LoadShaderProgram({ "res/04/main.vert", "res/04/main.frag" });
	std::vector<UniformInfo> uniformNames = {
		{ UniformProjectionMatrix, "uProjectionMatrix" },
		{ UniformWorldviewMatrix, "uWorldMatrix" },
		{ UniformColormap, "uColormap" },
	};
	std::vector<AttributeInfo> attributeNames = {
		{ AttributePosition, "aPosition" },
		{ AttributeTexCoord, "aTexCoord" },
	};
	m_shader.Reset(std::move(obj), uniformNames, attributeNames);
	m_states.SetProgram(m_shader);
}

void MainScene::Update(ps::seconds dt)
{
	// Обновляем местоположение космического корабля.
	const glm::vec2 direction = m_movementController.GetDirection();
	const glm::vec2 offset = direction * float(dt.count()) * SPACESHIP_VELOCITY;

	// Для вычисления поворота корабля можно перевести направление в полярные
	//  координаты, и угол в полярных координатах будет углом поворота корабля.
	// Воспользуемся функцией atan2.
	const float angleRadians = atan2(direction.y, direction.x);

	m_spaceship.transform.MoveBy(offset);
	m_spaceship.transform.orientation = angleRadians;
}

void MainScene::Draw(const sf::Window& window) const
{
	m_states.Rebind();
	m_states.SetCapabilityEnabled(gl::GL_BLEND, true);
	m_states.SetBlendFunction(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

	// Устанавливаем матрицу ортографического проецирования.
	const sf::Vector2u size = window.getSize();
	const glm::mat4 projMat = glm::ortho(0.f, float(size.x), float(size.y), 0.f);
	m_states.SetUniform(UniformProjectionMatrix, projMat);
	m_states.SetUniform(UniformColormap, gl::GL_TEXTURE0);

	// Рисуем космический корабль.
	DrawShape(m_spaceship);
}

bool MainScene::OnKeyPressed(const sf::Event::KeyEvent& event)
{
	return m_movementController.OnKeyPressed(event);
}

bool MainScene::OnKeyReleased(const sf::Event::KeyEvent& event)
{
	return m_movementController.OnKeyReleased(event);
}

void MainScene::DrawShape(const TexturedShape& shape) const
{
	m_states.SetTexture2D(gl::GL_TEXTURE0, shape.texture);
	m_states.SetUniform(UniformWorldviewMatrix, shape.transform.ToMat4());
	MeshUtils::DrawMesh(shape.mesh, m_states);
}
