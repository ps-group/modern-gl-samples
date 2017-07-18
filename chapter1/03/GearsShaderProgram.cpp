#include "GearsShaderProgram.h"
#include "GearsTesselator.h"
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>

using namespace gl33core;

// Этот вершинный шейдер выполняет две задачи:
//  1) передаёт цвет фрагментному шейдеру
//  2) выполняет преобразование вершин модели к мировым координатам
//  3) выполняет ортографическое проецирование вершины,
//     по сути проецируя вершину на плоскость экрана.
static const char kVertexShaderCode[] = R"**(#version 130
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
uniform mat4 u_world_matrix;
void main()
{
	v_color = i_color;
	gl_Position = u_projection_matrix * u_world_matrix * vec4( i_position, 0.0, 1.0 );
}
)**";

// Этот фрагментный шейдер устанавливает фрагменту переданный из
//  вершинного шейдера цвет.
static const char kFragmentShaderCode[] = R"**(#version 130
in vec4 v_color;
void main()
{
	gl_FragColor = v_color;
}
)**";

GearsShaderProgram::GearsShaderProgram()
{
	std::vector<ps::ShaderObject> shaders;
	shaders.emplace_back(ps::CompileShader(GL_VERTEX_SHADER, kVertexShaderCode));
	shaders.emplace_back(ps::CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode));

	m_program = ps::LinkProgram(shaders);
}

GearsShaderProgram::~GearsShaderProgram() = default;

void GearsShaderProgram::Bind()
{
	glUseProgram(m_program.get());
}

void GearsShaderProgram::BindAttributes(const ps::VertexBufferObject& vbo)
{
	// Привязываем буфер вершинных данных к текущему контексту.
	glBindBuffer(GL_ARRAY_BUFFER, vbo.get());

	// OpenGL должен получить байтовые смещения полей относительно структуры Vertex.
	const void* colorOffset = reinterpret_cast<void*>(offsetof(Vertex, rgba));
	const void* posOffset = reinterpret_cast<void*>(offsetof(Vertex, xy));
	const size_t stride = sizeof(Vertex);

	// Привязываем атрибут i_color к данным в вершинном буфере.
	const int colorLocation = glGetAttribLocation(m_program.get(), "i_color");
	assert(colorLocation != -1);
	glEnableVertexAttribArray(GLuint(colorLocation));
	glVertexAttribPointer(colorLocation, glm::vec4().length(), GL_FLOAT, GL_FALSE, stride, colorOffset);

	// Привязываем атрибут i_position к данным в вершинном буфере.
	const int posLocation = glGetAttribLocation(m_program.get(), "i_position");
	assert(posLocation != -1);
	glEnableVertexAttribArray(GLuint(posLocation));
	glVertexAttribPointer(posLocation, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
}

void GearsShaderProgram::SetProjectionMatrix(const glm::mat4& value)
{
	SetMatrixUniform("u_projection_matrix", value);
}

void GearsShaderProgram::SetWorldMatrix(const glm::mat4& value)
{
	SetMatrixUniform("u_world_matrix", value);
}

void GearsShaderProgram::SetMatrixUniform(const char name[], const glm::mat4& value)
{
	const int location = glGetUniformLocation(m_program.get(), name);
	assert(location != -1);

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
