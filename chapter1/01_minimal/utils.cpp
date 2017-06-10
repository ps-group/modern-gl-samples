#include "utils.h"
#include <glbinding/gl33core/gl.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace gl33core;

static std::string ReadInfoLog(gl::GLuint shader)
{
    // При неудаче есть лог ошибок, который мы соберём
    // и в первую очередь надо узнать длину лога.
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    // Зная длину, выделяем строку нужного размера и копируем в неё лог
    std::string log(logLength, '\0');
    GLsizei ignored = 0;
    glGetShaderInfoLog(shader, log.size(), &ignored, log.data());

    return log;
}

SDL_Window* CreateWindow(std::string_view title, int width, int height)
{
    return SDL_CreateWindow(
        title.data(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
}

// Компилирует шейдер заданного в `type` типа из строки с исходным кодом `sourceCode`
gl::GLuint CompileShader(gl::GLenum type, std::string_view sourceCode)
{
    // Выделяем ресурс шейдера
    GLuint shader = glCreateShader(type);
    try
    {
        // Передаём исходный код шейдера видеодрайверу
        const int length = static_cast<int>(sourceCode.length());
        const char *sourceCodePtr = sourceCode.data();
        glShaderSource(shader, 1, (const GLchar**)&sourceCodePtr, &length);

        // Просим видеодрайвер скомпилировать шейдер
        glCompileShader(shader);

        // Проверяем успешный/неуспешный статус компиляции
        GLboolean ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (ok == GL_FALSE)
        {
            const std::string log = ReadInfoLog(shader);

            // Бросаем исключение, прикрепив к нему лог
            throw std::runtime_error("shader compilation failed: " + log);
        }
    }
    catch (...)
    {
        // Освобождаем ресурс шейдера и перебрасываем исключение
        glDeleteShader(shader);
        throw;
    }

    return shader;
}

void DoEventLoop(SDL_Window* window, const std::function<void()>& draw)
{
    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                break;
            case SDL_QUIT:
                return;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        draw();

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
}

GLuint LinkProgram(GLuint vs, GLuint fs, const AttributeBinding& binding)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    for (const auto& pair : binding)
    {
        glBindAttribLocation(program, pair.first, pair.second.data());
    }

    glLinkProgram(program);

    return program;
}

glm::mat4 MakeOrthoMat4(float left, float right, float bottom, float top, float znear, float zfar)
{
    // По умолчанию матрица единичная
    glm::mat4 out;

    out[0][0] = 2.0f / (right - left);
    out[1][1] = 2.0f / (top - bottom);
    out[2][2] = -2.0f / (zfar - znear);
    out[3][0] = -(right + left) / (right - left);
    out[3][1] = -(top + bottom) / (top - bottom);
    out[3][2] = -(zfar + znear) / (zfar - znear);

    return out;
}

GLuint MakeVertexArray()
{
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    return vao;
}

GLuint MakeBuffer()
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    return vbo;
}

void UploadVertexData(const Vertex* data, size_t count)
{
    glEnableVertexAttribArray(attrib_color);
    glVertexAttribPointer(attrib_color, glm::vec4().length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(attrib_position);
    glVertexAttribPointer(attrib_position, glm::vec2().length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, xy));

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, data, GL_STATIC_DRAW);
}

void PrintOpenGLVersion()
{
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::cout << "OpenGL version: " << version << std::endl;
}
