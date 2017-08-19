#include "DebugTools.h"
#include <csignal>
#include <glbinding/Meta.h>
#include <glbinding/gl/gl.h>
#include <iostream>
#include <set>

using namespace gl;

// Макрос GLAPIENTRY раскрывается в атрибут __stdcall на платфоме Windows.
#if defined(_WIN32)
#define GLAPIENTRY __stdcall
#else
#define GLAPIENTRY
#endif

void GLAPIENTRY DebugOutputCallback(
	GLenum /*source*/,
	GLenum type,
	GLuint id,
	GLenum /*severity*/,
	GLsizei /*length*/,
	const GLchar* message,
	const void* /*userParam*/)
{
	// Отсекаем все сообщения, кроме ошибок
	if (type != GL_DEBUG_TYPE_ERROR)
	{
		return;
	}
	std::string formatted = "OpenGL error #" + std::to_string(id) + ": " + message;
	std::cerr << formatted << std::endl;
	PS_BREAKPOINT;
}

void ps::SetupDebugOutput()
{
	const auto extensions = glbinding::Meta::extensions();
	if (extensions.find(gl::GLextension::GL_ARB_debug_output) != extensions.end())
	{
		// Синхронный режим позволяет узнать в отладчике контекст,
		//  в котором произошла ошибка.
		// Режим может понизить производительность, но на фоне
		//  других потерь Debug-сборки это несущественно.
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

		// Устанавливаем процедуру для обработки отладочных сообщений.
		glDebugMessageCallbackARB(DebugOutputCallback, nullptr);

		// Указываем видеодрайверу отслеживать только один тип сообщений,
		//  GL_DEBUG_TYPE_ERROR.
		glDebugMessageControlARB(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
