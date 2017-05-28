#include "utils.h"
#include <glbinding/Binding.h>
#include <glbinding/Meta.h>
#include <glbinding/Version.h>
#include <glbinding/gl32core/gl.h>
#include <iostream>
#include <iterator>

using namespace gl32core;

static const char* vertex_shader = R"**(#version 130
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

static const char* fragment_shader = R"**(#version 130
in vec4 v_color;
out vec4 o_color;
void main()
{
    o_color = v_color;
}
)**";

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        static const int width = 800;
        static const int height = 600;

        InitSDL();

        SDL_Window* window = CreateWindow("Minimal OpenGL 3 Example", width, height);
        SDL_GLContext context = SDL_GL_CreateContext(window);

        glbinding::Binding::initialize();
        PrintOpenGLVersion();

        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
        const AttrBinding binding = {
            { attrib_position, "i_position" },
            { attrib_color, "i_color" },
        };
        GLuint program = LinkProgram(vs, fs, binding);

        glUseProgram(program);

        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 0.0, 0.0, 0.0);
        glViewport(0, 0, width, height);

        GLuint vao = MakeVertexArray();
        GLuint vbo = MakeBuffer();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        const Vertex vertexBufferData[] = {
            { { 1, 0, 0, 1 }, { 0, 0 } },
            { { 0, 1, 0, 1 }, { width, 0 } },
            { { 0, 0, 1, 1 }, { width, height } },
            { { 1, 0, 0, 1 }, { 0, 0 } },
            { { 0, 0, 1, 1 }, { width, height } },
            { { 1, 1, 1, 1 }, { 0, height } },
        };

        UploadVertexData(vertexBufferData, std::size(vertexBufferData));

        glm::mat4 projection_matrix = MakeOrthoMat4(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));

        DoEventLoop(window, [&]() {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        });

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "fatal error: " << ex.what() << std::endl;
    }

    return 0;
}
