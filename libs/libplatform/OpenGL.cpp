#include "OpenGL.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;
using namespace std;

namespace ps
{
namespace
{
string ReadShaderInfoLog(GLuint id)
{
	// При неудаче есть лог ошибок, который мы соберём
	// и в первую очередь надо узнать длину лога.
	GLint logLength = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// Зная длину, выделяем строку нужного размера и копируем в неё лог
	string log(logLength, '\0');
	GLsizei ignored = 0;
	glGetShaderInfoLog(id, log.size(), &ignored, log.data());

	return log;
}

string ReadProgramInfoLog(GLuint id)
{
	// При неудаче есть лог ошибок, который мы соберём
	// и в первую очередь надо узнать длину лога.
	GLint logLength = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// Зная длину, выделяем строку нужного размера и копируем в неё лог
	string log(logLength, '\0');
	GLsizei ignored = 0;
	glGetProgramInfoLog(id, log.size(), &ignored, log.data());

	return log;
}

void CheckShaderCompilatioStatus(GLuint id)
{
	GLboolean ok = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE)
	{
		// Бросаем исключение, прикрепив к нему лог
		const string log = ReadShaderInfoLog(id);
		throw runtime_error("shader compilation failed: "s + log);
	}
}

void CheckProgramLinkingStatus(GLuint id)
{
	GLboolean status = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Бросаем исключение, прикрепив к нему лог
		const string log = ReadProgramInfoLog(id);
		throw runtime_error("program linking failed "s + log);
	}
}
} // anonymous namespace

ShaderObject CompileShader(gl::GLenum type, string_view source)
{
	// Выделяем ресурс шейдера
	ShaderObject obj = ShaderObject(glCreateShader(type));

	// Передаём исходный код шейдера видеодрайверу
	const auto length = static_cast<int>(source.length());
	const char* sourceLine = source.data();
	glShaderSource(obj.get(), 1, (const GLchar**)&sourceLine, &length);

	// Просим видеодрайвер скомпилировать шейдер и проверяем статус
	glCompileShader(obj.get());
	CheckShaderCompilatioStatus(obj.get());

	return obj;
}

ProgramObject LinkProgram(const std::vector<ShaderObject>& shaders)
{
	// Запрашиваем у видеодрайера новый объект.
	ProgramObject obj = ProgramObject(glCreateProgram());

	// Прикрепляем ранее скомпилированные шейдеры.
	for (const ShaderObject& shader : shaders)
	{
		glAttachShader(obj.get(), shader.get());
	}

	// Просим видеодрайвер выполнить компоновку и проверяем статус.
	glLinkProgram(obj.get());
	CheckProgramLinkingStatus(obj.get());

	// Отсоединяем шейдеры, поскольку программа уже собрана.
	for (const ShaderObject& shader : shaders)
	{
		glDetachShader(obj.get(), shader.get());
	}

	return obj;
}

void ValidateProgram(const ProgramObject& program, std::ostream& out)
{
	glValidateProgram(program.get());

	GLboolean status = 0;
	glGetProgramiv(program.get(), GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Записываем лог валидации в поток
		const string log = ReadProgramInfoLog(program.get());
		out << log;
	}
}

VertexArrayObject CreateVAO()
{
	GLuint handle = 0;
	glGenVertexArrays(1, &handle);
	if (handle == 0)
	{
		throw std::runtime_error("cannot create VAO: no enough memory");
	}
	return VertexArrayObject(handle);
}

VertexBufferObject CreateStaticVBO(gl::GLenum target, const byte* bytes, const size_t byteCount)
{
	GLuint handle = 0;
	glGenBuffers(1, &handle);
	if (handle == 0)
	{
		throw std::runtime_error("cannot create VAO: no enough memory");
	}

	glBindBuffer(target, handle);
	glBufferData(target, byteCount, bytes, GL_STATIC_DRAW);

	return VertexBufferObject(handle);
}

TextureObject CreateTexture()
{
	gl::GLuint handle = 0;
	glGenTextures(1, &handle);
	if (handle == 0)
	{
		throw std::runtime_error("cannot create VAO: no enough memory");
	}
	return TextureObject(handle);
}

void ValidateOpenGLError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string message;
		switch (error)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
			break;
		case GL_INVALID_VALUE:
			message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
			break;
		case GL_INVALID_OPERATION:
			message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
			break;
		case GL_OUT_OF_MEMORY:
			message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
			break;
		default:
			message = "error in some GL extension (framebuffers, shaders, etc)";
			break;
		}
		throw std::runtime_error("OpenGL error: " + message);
	}
}
}
