#include "GearsScene.h"
#include "GearsShaderProgram.h"
#include "GearsTesselator.h"
#include "libmath/CommonMath.h"
#include <glbinding/gl33core/gl.h>
#include <glm/mat4x4.hpp>

using namespace gl33core;

constexpr float kGearRotationPeriod = 6.f;
constexpr float kGearAngularVelocity = float(2 * M_PI) / kGearRotationPeriod;

// Превращает rgb(255, 0, 128) в vec4{ 1, 0, 0.5, 1 }
glm::vec4 MakeRGB(unsigned red, unsigned green, unsigned blue)
{
	return glm::vec4(float(red) / 255.f, float(green) / 255.f, float(blue) / 255.f, 1);
};

GearsScene::GearsScene()
{
	// Создаём Vertex Array Object (VAO), который хранит связи между данными
	//  в VBO и переменными шейдера.
	m_vao = ps::CreateVAO();
	glBindVertexArray(m_vao.get());

	// Инициализируем первую шестерёнку
	{
		// Генерируем список вершин треугольников, представляющих шестерню.
		std::vector<Vertex> verticies = GearsTesselator::TesselateGear(
			100.f, 170.f, 8, 30.f, { 0, 0 }, MakeRGB(204, 153, 51));

		m_gear1.vertexCount = verticies.size();

		// Создаём Vertex Buffer Object (VBO) для загрузки данных,
		//  в этот буфер мы запишем параметры вершин для видеокарты.
		m_gear1.vbo = ps::CreateStaticVBO(GL_ARRAY_BUFFER, verticies);

		// Устанавливаем начальное положение шестерёнки.
		m_gear1.transform.MoveBy({ 250, 295 });
	}

	// Инициализируем вторую шестерёнку
	{
		// Генерируем список вершин треугольников, представляющих шестерню.
		std::vector<Vertex> verticies = GearsTesselator::TesselateGear(
			100.f, 150.f, 8, 30.f, { 0, 0 }, MakeRGB(102, 153, 153));

		m_gear2.vertexCount = verticies.size();
		m_gear2.vbo = ps::CreateStaticVBO(GL_ARRAY_BUFFER, verticies);
		m_gear2.transform.MoveBy({ 520, 190 });
	}
}

GearsScene::~GearsScene() = default;

void GearsScene::Update(ps::seconds dt)
{
	if (m_rotationEnabled)
	{
		const float rotation = kGearAngularVelocity * dt.count();
		m_gear1.transform.RotateBy(rotation);
		m_gear2.transform.RotateBy(-rotation);
	}
}

void GearsScene::Draw(GearsShaderProgram& program) const
{
	// На каждом кадре привязываем VAO
	glBindVertexArray(m_vao.get());

	// Рисуем первую шестерёнку
	//  - устанавливаем матрицу преобразования в глобальные координаты
	//  - привязываем буфер с вершинными данными
	//  - рисуем треугольники, используя вершины
	//    из полуинтервала [0, verticies.size)
	program.SetWorldMatrix(m_gear1.transform.ToMat4());
	program.BindAttributes(m_gear1.vbo);
	glDrawArrays(GL_TRIANGLES, 0, m_gear1.vertexCount);

	// Рисуем вторую шестерёнку
	program.SetWorldMatrix(m_gear2.transform.ToMat4());
	program.BindAttributes(m_gear2.vbo);
	glDrawArrays(GL_TRIANGLES, 0, m_gear2.vertexCount);
}

bool GearsScene::OnMousePressed(const sf::Event::MouseButtonEvent& event)
{
	if (event.button == sf::Mouse::Button::Left)
	{
		m_rotationEnabled = false;
		return true;
	}
	return false;
}

bool GearsScene::OnMouseReleased(const sf::Event::MouseButtonEvent& event)
{
	if (event.button == sf::Mouse::Button::Left)
	{
		m_rotationEnabled = true;
		return true;
	}
	return false;
}
