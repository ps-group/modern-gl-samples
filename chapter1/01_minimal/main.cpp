#include "utils.h"
#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iterator>

using namespace gl33core;

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

static const char kFragmentShaderCode[] = R"**(#version 130
in vec4 v_color;
out vec4 o_color;
void main()
{
    o_color = v_color;
}
)**";

int main(int /*argc*/, char* /*argv*/[])
{
    try
    {
        static const int width = 800;
        static const int height = 600;

        SDL_Init(SDL_INIT_VIDEO);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        SDL_Window* window = CreateWindow("Minimal OpenGL 3 Example", width, height);
        SDL_GLContext context = SDL_GL_CreateContext(window);

        glbinding::Binding::initialize();
        PrintOpenGLVersion();

        GLuint vs = CompileShader(GL_VERTEX_SHADER, kVertexShaderCode);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode);
        const AttributeBinding binding = {
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
