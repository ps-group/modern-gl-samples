#include "utils.h"
#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iterator>

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

int main(int /*argc*/, char* /*argv*/[])
{
    try
    {
        static const int width = 800;
        static const int height = 600;

        // Инициализируем SDL2.
        SDL_Init(SDL_INIT_VIDEO);

        // Указываем SDL2, что контекст должен иметь определённые атрибуты
        //  - версия OpenGL 3.3 Core Profile
        //  - двойная буферизация кадра
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Создаём окно с заданным заголовком и размерами
        SDL_Window* window = CreateWindow("Minimal OpenGL 3 Example", width, height);
        // Создаём контекст OpenGL для полученного окна.
        SDL_GLContext context = SDL_GL_CreateContext(window);
        if (context == nullptr)
        {
            throw std::runtime_error("OpenGL initialization failed");
        }

        // Инициализируем библиотеку glbinding после создания контекста OpenGL
        glbinding::Binding::initialize();
        PrintOpenGLVersion();

        // Компилируем вершинный и фрагментный шейдеры
        GLuint vs = CompileShader(GL_VERTEX_SHADER, kVertexShaderCode);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode);

        // Выполняем связывание (linking) шейдеров в единую графическую программу.
        const AttributeBindingMap binding = {
            { attrib_position, "i_position" },
            { attrib_color, "i_color" },
        };
        GLuint program = LinkProgram(vs, fs, binding);

        // Активируем программу в текущем контексте.
        glUseProgram(program);

        // Отключаем ненужный в 2D тест глубины
        glDisable(GL_DEPTH_TEST);
        // Устанавливаем RGBA цвет очистки экрана для glClear().
        glClearColor(0, 0.0, 0.0, 0.0);
        // Устанавливаем порт просмотра (размер равен размеру внутренней области окна)
        glViewport(0, 0, width, height);

        // Создаём Vertex Array Object,
        //  хотя бы один VAO должен быть активен при рисовании.
        GLuint vao = MakeVertexArray();

        // Создаём Vertex Buffer Object для загрузки данных,
        //  в этот буфер мы запишем параметры вершин для видеокарты.
        GLuint vbo = MakeBuffer();

        // Активируем VAO и VBO
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Шесть вершин описывают два треугольника,
        //  которые вместе составляют прямоугольник.
        const Vertex vertexBufferData[] = {
            { { 1, 0, 0, 1 }, { 0, 0 } },
            { { 0, 1, 0, 1 }, { width, 0 } },
            { { 0, 0, 1, 1 }, { width, height } },
            { { 1, 0, 0, 1 }, { 0, 0 } },
            { { 0, 0, 1, 1 }, { width, height } },
            { { 1, 1, 1, 1 }, { 0, height } },
        };

        // Загружаем вершинные данные на видеокарту.
        UploadVertexData(vertexBufferData, std::size(vertexBufferData));

        // Создаём матрицу ортографической проекции.
        glm::mat4 projection_matrix = MakeOrthoMat4(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
    
        // Передаём матрицу как константу в графической программе
        glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));

        // Запускаем цикл событий
        DoEventLoop(window, [&]() {
            // На каждом кадре привязываем VAO
            glBindVertexArray(vao);
            // Рисуем треугольники, используя вершины из полуинтервала [0, 6)
            glDrawArrays(GL_TRIANGLES, 0, 6);
        });

        // Очищаем контекст, окно и внутренние данные SDL2.
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    catch (const std::exception& ex)
    {
        // В случае фатального исключения показываем MessageBox.
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!",
                                 ex.what(), nullptr);
    }

    return 0;
}
