#pragma once
#include "Enumerations.h"
#include "IShaderProgram.h"
#include "UniformValue.h"
#include "libplatform/OpenGL.h"
#include <bitset>

// Класс отслеживает состояние некоторых свойств контекста OpenGL:
//  - активный шейдер
//  - активные текстуры
//  - активные буферы вершин и индексов
//  - активные атрибуты вершин
// Для простоты используется один Vertex Array Object (VAO).
class RenderStates
{
public:
	// Конструктор инициализирует и привязывает Vertex Array Object.
	RenderStates();

	// Копирование и перемещение запрещено.
	RenderStates(const RenderStates&) = delete;
	RenderStates& operator=(const RenderStates&) = delete;
	RenderStates(RenderStates&&) = delete;
	RenderStates& operator=(RenderStates&&) = delete;

	void Rebind();

	// Фасад функции glDrawArrays, запускает в текущем контексте один
	//  проход рисования (draw batch).
	// @param primitive - тип примитива: обычно GL_POINTS, GL_LINES
	//  или GL_TRIANGLES.
	// @param first - индекс первой вершины в интервале для рисования.
	// @param count - количество вершин в интервале для рисования.
	void DrawArrays(gl::GLenum primitive, size_t first, size_t count);

	// Фасад функции glDrawRangeElements, запускает в текущем контексте
	//  один проход рисования (draw batch).
	// @param primitive - тип примитива: обычно GL_POINTS, GL_LINES
	//  или GL_TRIANGLES.
	// @param minIndex - минимальный индекс вершины в буфере индексов.
	// @param maxIndex - максимальный индекс вершины в буфере индексов.
	// @param indexCount - количество индексов.
	// @param indexType - тип индекса, обычно GL_UNSIGNED_INT.
	// @param firstIndexOffset - смещение в байтах от начала буфера
	//  до первого индекса.
	void DrawElements(
		gl::GLenum primitive, unsigned minIndex, unsigned maxIndex,
		size_t indexCount, gl::GLenum indexType, size_t firstIndexOffset = 0);

	void SetProgram(const IShaderProgram& shader);

	// Привязывает буфер атрибутов вершин к слоту GL_ARRAY_BUFFER.
	void BindVerticiesBuffer(const ps::VertexBufferObject& vbo);

	// Привязывает буфер индексов вершин к слоту GL_ELEMENT_ARRAY_BUFFER.
	void BindIndiciesBuffer(const ps::VertexBufferObject& vbo);

	// Привязывает текстуру к указанному текстурному слоту.
	void SetTexture2D(TextureId id, const ps::TextureObject& texture);

	// Привязывает к uniform-переменной шейдера значение,
	//  хранимое в вариантном типе UniformValue.
	void SetUniform(UniformId id, const UniformValue& value);

	// Фасад функций glEnableVertexAttribArray и glDisableVertexAttribArray,
	//  включает чтение указанных входных переменных шейдера
	//  из активного буфера типа GL_ARRAY_BUFFER,
	//  и выключает чтение остальных входных переменных.
	// После включения атрибута нужно установить способ чтения вызомом
	//  SetVertexAttribOffset (или glVertexAttribPointer)
	void ActivateAttributes(std::initializer_list<AttributeId> ids);

	// Фасад функции glVertexAttribPointer, устанавливает привязку
	//  входной переменной вершинного шейдера и interleaved array
	//  внутри активного буфера типа GL_ARRAY_BUFFER.
	// @param id - идентификатор переменной шейдера
	// @param dimensions - размерность векторной величины атрибута,
	//	например 2 для vec2 или 4 для vec4
	// @param type - тип компонентов векторной величины атрибута, обычно GL_FLOAT
	// @param clamped - GL_TRUE если компоненты атрибута надо привести
	//  к диапазону [0..1], иначе GL_FALSE
	// @param size - размер элемента буфера в байтах
	// @param offset - смещение в байтах от начала элемента буфера до поля,
	//  хранящего векторную величину
	void SetAttributeOffset(
		AttributeId id, gl::GLuint dimensions, gl::GLenum type,
		gl::GLboolean clamped, size_t size, size_t offset);

private:
	void ThrowIfNoShader();
	void ThrowIfNoVerticiesBuffer();
	void ThrowIfNoIndiciesBuffer();
	const IShaderProgram& GetShader();

	const IShaderProgram* m_activeProgram = nullptr;
	ps::VertexArrayObject m_vao;
	gl::GLuint m_verticiesVBO = 0;
	gl::GLuint m_indiciesVBO = 0;
	std::array<gl::GLuint, TextureIdCount> m_activeTexture = { 0 };
	std::bitset<AttributeIdCount> m_activeAttributes;
};
