#define _USE_MATH_DEFINES
#include <cmath>

#include "RandomColorGenerator.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <random>
#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <iostream>

using namespace gl33core;

// Этот вершинный шейдер выполняет две задачи:
//  1) передаёт цвет фрагментному шейдеру
//  2) выполняет ортографическое проецирование вершины,
//     по сути проецируя вершину на плоскость экрана.
static const char kVertexShaderCode[] = R"**(#version 130
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
void main()
{
    v_color = i_color;
    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );
}
)**";

// Этот фрагментный шейдер устанавливает фрагменту переданный из
//  вершинного шейдера цвет.
static const char kFragmentShaderCode[] = R"**(#version 130
in vec4 v_color;
out vec4 o_color;
void main()
{
    gl_FragColor = v_color;
}
)**";

using DrawCallback = std::function<void()>;
using GenerateColorFn = std::function<glm::vec4()>;

struct Vertex
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

// Переводит полярные координаты {radius, angle} в декартовы.
// Угол задаётся в радианах.
glm::vec2 euclidean(float radius, float angle)
{
	return { radius * cos(angle), radius * sin(angle) };
}

void BindVertexData(const std::vector<Vertex>& verticies, const sf::Shader& shader)
{
	const GLuint programId = shader.getNativeHandle();

	// OpenGL должен получить байтовые смещения полей относительно структуры Vertex.
	const void* colorOffset = reinterpret_cast<void*>(offsetof(Vertex, rgba));
	const void* posOffset = reinterpret_cast<void*>(offsetof(Vertex, xy));
	const size_t stride = sizeof(Vertex);

	// Привязываем атрибут i_color к данным в вершинном буфере.
	const int colorLocation = glGetAttribLocation(programId, "i_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, glm::vec4().length(), GL_FLOAT, GL_FALSE, stride, colorOffset);

	// Привязываем атрибут i_position к данным в вершинном буфере.
	const int posLocation = glGetAttribLocation(programId, "i_position");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, posOffset);

	// Загружаем данные в вершинный буфер.
	glBufferData(GL_ARRAY_BUFFER, stride * verticies.size(), verticies.data(), GL_STATIC_DRAW);
}

void SetProjectionMatrix(const sf::Window& window, const sf::Shader& shader)
{
	// Вычисляем матрицу ортографического проецирования
	const glm::mat4 mat = glm::ortho(0.f, float(window.getSize().x), float(window.getSize().y), 0.f);
	const GLuint programId = shader.getNativeHandle();

	// Передаём матрицу как константу в графической программе
	glUniformMatrix4fv(glGetUniformLocation(programId, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами и центром.
//  @param center - геометрический центр многоугольника
//  @param hullPoints - вершины многоугольника
//  @param colorGen - генератор цвета полученных треугольников
std::vector<Vertex> TesselateConvexByCenter(const glm::vec2& center, const std::vector<glm::vec2>& hullPoints, IColorGenerator& colorGen)
{
	const size_t size = hullPoints.size();
	std::vector<Vertex> verticies(3u * size);
	for (size_t pointIndex = 0; pointIndex < size; ++pointIndex)
	{
		// Генерируем цвет треугольника и добавляем три его вершины в список.
		const glm::vec4 triangleColor = colorGen.GenerateColor();
		const size_t nextPointIndex = (pointIndex + 1) % size;
		verticies.push_back({ hullPoints.at(pointIndex), triangleColor });
		verticies.push_back({ hullPoints.at(nextPointIndex), triangleColor });
		verticies.push_back({ center, triangleColor });
	}

	return verticies;
}

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
std::vector<Vertex> TesselateConvex(const std::vector<glm::vec2>& verticies, IColorGenerator& colorGen)
{
	// Центр выпуклого многоугольника - это среднее арифметическое его вершин
	const glm::vec2 center = std::reduce(verticies.begin(), verticies.end()) / float(verticies.size());
	return TesselateConvexByCenter(center, verticies, colorGen);
}

// Функция делит круг на треугольники,
//  возвращает массив с вершинами треугольников.
std::vector<Vertex> TesselateCircle(float radius, const glm::vec2& center, IColorGenerator& colorGen)
{
	assert(radius > 0);

	// Круг аппроксимируется с помощью треугольников.
	// Внешняя сторона каждого треугольника имеет длину 2.
	constexpr float step = 2;
	// Число треугольников равно длине окружности, делённой на шаг по окружности.
	const auto pointCount = static_cast<unsigned>(radius * 2 * M_PI / step);

	// Вычисляем точки-разделители на окружности.
	std::vector<glm::vec2> points(pointCount);
	for (unsigned pi = 0; pi < pointCount; ++pi)
	{
		const auto angleRadians = static_cast<float>(2 * M_PI * pointCount / pi);
		points[pi] = center + euclidean(radius, angleRadians);
	}
	
	return TesselateConvexByCenter(center, points, colorGen);
}

// Исполняет цикл рисования, завершается при закрытии окна.
void EnterRenderLoop(sf::Window& window, const DrawCallback& draw)
{
	glClearColor(0, 0, 0, 0);
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
		glClear(GL_COLOR_BUFFER_BIT);
		draw();
		// Вывод кадра на экран
		window.display();
	}
}

int main()
{
	try
	{
		// Указываем SFML, что контекст должен иметь определённые атрибуты
		//  - версия OpenGL 3.3 Core Profile
		//  - уровень сглаживания: 4
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		settings.attributeFlags = sf::ContextSettings::Core;
		settings.majorVersion = 3;
		settings.minorVersion = 3;

		// Создаём окно SFML
		sf::Window window(
			sf::VideoMode(800, 600), "OpenGL shapes",
			sf::Style::Default, settings);

		// Инициализируем библиотеку glbinding после создания контекста OpenGL
		glbinding::Binding::initialize();

		// Компилируем вершинный и фрагментный шейдеры в шейдерную программу
		sf::Shader shader;
		if (!shader.loadFromMemory(kVertexShaderCode, kFragmentShaderCode))
		{
			throw std::runtime_error("vertex or fragment shader compilation failed");
		}

		// Отключаем ненужный в 2D тест глубины
		glDisable(GL_DEPTH_TEST);
		// Устанавливаем порт просмотра (размер равен размеру внутренней области окна)
		glViewport(0, 0, window.getSize().x, window.getSize().y);

		// Создаём Vertex Array Object,
		//  хотя бы один VAO должен быть активен при рисовании.
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);

		// Создаём Vertex Buffer Object для загрузки данных,
		//  в этот буфер мы запишем параметры вершин для видеокарты.
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);

		// Активируем VAO и VBO и шейдер в текущем контексте OpenGL.
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		sf::Shader::bind(&shader);

		// Генерируем список вершин треугольников, представляющих круг,
		//  каждый треугольник будет раскрашен в собственный цвет.
		RandomColorGenerator colorGen;
		std::vector<Vertex> verticies = TesselateCircle(50, { 350, 280 }, colorGen);

		// Добавляем к списку вершин вершины многоугольника.
		const std::vector<glm::vec2> convexPoints = {
			{ 100, 200 },
			{ 250, 210 },
			{ 220, 290 },
			{ 130, 300 },
			{ 100, 250 },
		};
		const std::vector<Vertex> convexVerticies = TesselateConvex(convexPoints, colorGen);
		std::copy(convexVerticies.begin(), convexVerticies.end(), std::back_inserter(verticies));

		// Выполняем привязку данных в контексте текущего VAO.
		BindVertexData(verticies, shader);

		SetProjectionMatrix(window, shader);

		EnterRenderLoop(window, [&]() {
			// На каждом кадре привязываем VAO
			glBindVertexArray(vao);
			// Рисуем треугольники, используя вершины из полуинтервала [0, verticies.size)
			glDrawArrays(GL_TRIANGLES, 0, verticies.size());
		});
	}
	catch (const std::exception& ex)
	{
		std::cerr << "fatal exception: " << ex.what() << std::endl;
	}

	return 0;
}
