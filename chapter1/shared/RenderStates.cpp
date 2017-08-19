#include "RenderStates.h"
#include <glbinding/Meta.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>

using namespace gl33core;

// Класс реализует установку значения uniform-переменной подходящим вызовом
//  в зависимости от типа значения.
struct SetUniformVisitor
{
	SetUniformVisitor(int location)
		: m_location(location)
	{
	}

	// Uniform-переменная типа Sampler2D должна получить индекс слота,
	//  к которому привязана текстура с данными.
	// Первый индекс слота - 0.
	void operator()(TextureId value)
	{
		glUniform1i(m_location, static_cast<int>(value));
	}

	void operator()(float value)
	{
		glUniform1f(m_location, value);
	}

	void operator()(const glm::vec2& value)
	{
		glUniform2fv(m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::vec3& value)
	{
		glUniform3fv(m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::vec4& value)
	{
		glUniform4fv(m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::mat3& value)
	{
		glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void operator()(const glm::mat4& value)
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	const int m_location;
};

RenderStates::RenderStates()
{
	m_vao = ps::CreateVAO();
	glBindVertexArray(m_vao.get());
}

void RenderStates::Rebind()
{
	glBindVertexArray(m_vao.get());
}

void RenderStates::SetProgram(const IShaderProgram& shader)
{
	if (std::addressof(shader) != m_activeProgram)
	{
		m_activeProgram = std::addressof(shader);
		m_activeProgram->Bind();
	}
}

void RenderStates::BindVerticiesBuffer(const ps::VertexBufferObject& vbo)
{
	if (m_verticiesVBO != vbo.get())
	{
		m_verticiesVBO = vbo.get();
		glBindBuffer(GL_ARRAY_BUFFER, m_verticiesVBO);
	}
}

void RenderStates::BindIndiciesBuffer(const ps::VertexBufferObject& vbo)
{
	if (m_indiciesVBO != vbo.get())
	{
		m_indiciesVBO = vbo.get();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indiciesVBO);
	}
}

void RenderStates::DrawArrays(gl::GLenum primitive, size_t first, size_t count)
{
	// Проверяем, что состояние контекста настроено.
	ThrowIfNoVerticiesBuffer();
	ThrowIfNoShader();

	glDrawArrays(primitive, GLint(first), GLsizei(count));
}

void RenderStates::DrawElements(gl::GLenum primitive, unsigned minIndex, unsigned maxIndex,
	size_t indexCount, gl::GLenum indexType, size_t firstIndexOffset)
{
	// Проверяем, что состояние контекста настроено.
	ThrowIfNoIndiciesBuffer();
	ThrowIfNoVerticiesBuffer();
	ThrowIfNoShader();

	// Указатель используется вместо целочисленной переменной
	//  по историческим причинам: ранее семейство функций glDrawElements
	//  принимало указатель на область оперативной памяти,
	//  а сейчас принимает смещение от начала буфера в видеопамяти.
	const auto* pointer = reinterpret_cast<const void*>(firstIndexOffset);

	// glDrawRangeElements эффективнее, чем glDrawElements,
	//  т.к. ему передаются максимальный и минимальный индекс вершины,
	//  что позволяет видеокарте определить и подготовить область памяти
	//  с вершинными данными.
	glDrawRangeElements(primitive, GLuint(minIndex), GLuint(maxIndex), GLsizei(indexCount), indexType, pointer);
}

void RenderStates::SetTexture2D(TextureId id, const ps::TextureObject& texture)
{
	GLuint textureId = texture.get();
	const auto slot = static_cast<int>(id);

	if (m_activeTexture.at(slot) != textureId)
	{
		// Привязываем текстуру как активную к заданному слоту,
		//  после привязки мы должны вновь активировать слот GL_TEXTURE0,
		//  иначе состояние OpenGL при рисовании будет сломанным.
		// См. https://stackoverflow.com/questions/3569261
		if (slot == 0)
		{
			glBindTexture(GL_TEXTURE_2D, textureId);
		}
		else
		{
			glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + slot));
			glBindTexture(GL_TEXTURE_2D, textureId);
			glActiveTexture(GL_TEXTURE0);
		}
		m_activeTexture.at(slot) = textureId;
	}
}

void RenderStates::SetUniform(UniformId id, const UniformValue& value)
{
	auto& shader = GetShader();

	// Получаем местоположение (номер слота) uniform-переменной шейдера.
	const int location = shader.GetUniform(id);

	// Если переменная, присваиваем ей значение.
	if (location != -1)
	{
		SetUniformVisitor visitor(location);
		std::visit(visitor, value);
	}
}

void RenderStates::ActivateAttributes(std::initializer_list<AttributeId> ids)
{
	auto& shader = GetShader();

	// Заносим в bitset атрибуты, которые должны быть активными.
	std::bitset<AttributeIdCount> newActiveAttributes;
	for (auto id : ids)
	{
		newActiveAttributes.set(id, true);
	}

	// Перебираем все атрибуты и просим видеодрайвер включить/выключить
	//  атрибут, если его состояние должно измениться.
	for (size_t id = 0; id < AttributeIdCount; ++id)
	{
		// Проверяем, что состояние атрибута изменилось.
		if (m_activeAttributes.test(id) != newActiveAttributes.test(id))
		{
			// Проверяем, что шейдер поддерживает атрибут.
			const int location = shader.GetAttribute(AttributeId(id));
			if (location != -1)
			{
				// Меняем состояние атрибута на требуемое.
				if (newActiveAttributes.test(id))
				{
					glEnableVertexAttribArray(GLuint(location));
				}
				else
				{
					glDisableVertexAttribArray(GLuint(location));
				}
			}
		}
	}

	m_activeAttributes = newActiveAttributes;
}

void RenderStates::SetAttributeOffset(
	AttributeId id, gl::GLuint dimensions, gl::GLenum type,
	gl::GLboolean clamped, size_t size, size_t offset)
{
	auto& shader = GetShader();

	// Проверяем, что атрибут был активирован ранее.
	if (!m_activeAttributes.test(id))
	{
		throw std::logic_error("attribute must be actived before set offset");
	}

	// Получаем местоположение (номер слота) атрибута, то есть
	//  входной переменной вершинного шейдера.
	const int location = shader.GetAttribute(id);

	// Если атрибут существует, указываем параметры привязки данных буфера.
	if (location != -1)
	{
		// Из-за наследия прошлого OpenGL принимает offset в виде указателя.
		// В прошлом pointer хранил адрес массива данных в оперативной памяти,
		//  а в современном OpenGL он хранит смещение (в байтах)
		//  от начала каждого элемента массива до атрибута.
		const auto* pointer = reinterpret_cast<const void*>(offset);

		// Передаём параметры привязки видеодрайверу.
		glVertexAttribPointer(
			GLuint(location), GLint(dimensions), type,
			clamped, GLsizei(size), pointer);
	}
}

void RenderStates::ThrowIfNoShader()
{
	if (m_activeProgram == nullptr)
	{
		throw std::logic_error("render state error: no shader program binded");
	}
}

void RenderStates::ThrowIfNoVerticiesBuffer()
{
	if ((m_verticiesVBO == 0) || (m_indiciesVBO == 0))
	{
		throw std::logic_error("render state error: no vertex buffer binded");
	}
}

void RenderStates::ThrowIfNoIndiciesBuffer()
{
	if ((m_verticiesVBO == 0) || (m_indiciesVBO == 0))
	{
		throw std::logic_error("render state error: no vertex buffer binded");
	}
}

const IShaderProgram& RenderStates::GetShader()
{
	ThrowIfNoShader();
	return *m_activeProgram;
}
