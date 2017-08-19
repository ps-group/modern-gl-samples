#pragma once
#include <cstddef>
#include <glbinding/gl/types.h>

namespace ps::detail
{
using DeleteFn = void(gl::GLuint handle) noexcept;

void DeleteShader(gl::GLuint handle) noexcept;
void DeleteProgram(gl::GLuint handle) noexcept;
void DeleteVBO(gl::GLuint handle) noexcept;
void DeleteVAO(gl::GLuint handle) noexcept;
void DeleteTexture(gl::GLuint handle) noexcept;

template<DeleteFn deleteFn>
class GfxObject
{
public:
	// Копирование объекта запрещено (можно только переместить).
	GfxObject(const GfxObject& other) = delete;
	GfxObject& operator=(const GfxObject& other) = delete;

	// По умолчанию конструируется с нулевым объектом
	GfxObject() = default;

	// Можно конструировать из числового идентификатора объекта OpenGL
	explicit GfxObject(gl::GLuint handle)
		: m_handle(handle)
	{
	}

	// Можно переместить объект (согласно move-семантике)
	GfxObject(GfxObject&& other) noexcept
	{
		std::swap(m_handle, other.m_handle);
	}

	// Можно переместить объект (согласно move-семантике)
	GfxObject& operator=(GfxObject&& other) noexcept
	{
		reset();
		std::swap(m_handle, other.m_handle);
		return *this;
	}

	// При уничтожении хранимый объект также удаляется
	~GfxObject()
	{
		reset();
	}

	// reset удаляет хранимый объект, если он есть.
	void reset() noexcept
	{
		if (m_handle != 0)
		{
			deleteFn(m_handle);
			m_handle = 0;
		}
	}

	// get даёт доступ к числовому идентификатору объекта OpenGL
	gl::GLuint get() const noexcept
	{
		return m_handle;
	}

	// при приведении к типу bool происходит проверка - хранится ли объект?
	explicit operator bool() const noexcept
	{
		return (m_handle != 0);
	}

private:
	gl::GLuint m_handle = 0;
};
}
