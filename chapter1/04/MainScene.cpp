#include "MainScene.h"
#include "../shared/AssetLoader.h"
#include "MeshUtils.h"
#include <glm/gtc/matrix_transform.hpp>

namespace
{
MeshP2T2 MakeSpriteMesh()
{
	const math::FloatRect rect({ -0.5f, -0.5f }, { 0.5f, 0.5f });
	return MeshUtils::BuildMesh(MeshUtils::TesselateQuad(rect));
}
}

MainScene::MainScene()
{
	TexturedShape shape;
	shape.texture = AssetLoader::LoadTexture2D("res/04/spaceship.png");
	shape.mesh = MakeSpriteMesh();
	shape.transform.ScaleBy(100);
	shape.transform.MoveBy({ 200, 250 });
	m_shapes.emplace_back(std::move(shape));

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
}

void MainScene::Draw(const sf::Window& window) const
{
	m_states.Rebind();

	// Устанавливаем матрицу ортографического проецирования.
	const sf::Vector2u size = window.getSize();
	const glm::mat4 projMat = glm::ortho(0.f, float(size.x), float(size.y), 0.f);
	m_states.SetUniform(UniformProjectionMatrix, projMat);
	m_states.SetUniform(UniformColormap, Texture0);

	for (const auto& shape : m_shapes)
	{
		m_states.SetTexture2D(Texture0, shape.texture);
		m_states.SetUniform(UniformWorldviewMatrix, shape.transform.ToMat4());
		MeshUtils::DrawMesh(shape.mesh, m_states);
	}
}

bool MainScene::OnMousePressed(const sf::Event::MouseButtonEvent& event)
{
	(void)event;
	return false;
}

bool MainScene::OnMouseReleased(const sf::Event::MouseButtonEvent& event)
{
	(void)event;
	return false;
}
