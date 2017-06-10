#include "libplatform/EventLoop.h"
#include "libplatform/sdlcpp.h"
#include "libshade/Program.h"
#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>

using namespace shade;
using namespace gl33core;
using namespace std::string_view_literals;

static const std::string_view kVertexShaderCode = R"**(#version 130
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
void main()
{
    v_color = i_color;
    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );
}
)**"sv;

static const std::string_view kFragmentShaderCode = R"**(#version 130
in vec4 v_color;
out vec4 o_color;
void main()
{
    o_color = v_color;
}
)**"sv;

struct Vertex
{
    glm::vec4 rgba;
    glm::vec2 xy;
};

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

void UploadVertexData(shade::IProgram& program, const Vertex* data, size_t count)
{
    auto colorAttr = program.GetAttribute(AttributeId::COLOR);
    colorAttr.Enable();
    colorAttr.SetFloatsOffset(offsetof(Vertex, rgba), sizeof(Vertex), 4);

    auto positionAttr = program.GetAttribute(AttributeId::POSITION);
    positionAttr.Enable();
    positionAttr.SetFloatsOffset(offsetof(Vertex, xy), sizeof(Vertex), 2);

    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(Vertex) * count), data, GL_STATIC_DRAW);
}

void RunApp()
{
    static const int width = 800;
    static const int height = 600;

    sdl::InitSDL init;
    sdl::SetGraphicsAttributes(sdl::GraphicsAttributes());

    auto window = sdl::CreateWindow("Minimal OpenGL 3 Example", { width, height });
    auto context = sdl::CreateGraphicsContext(*window);

    glbinding::Binding::initialize();

    shade::Program program;
    program.SetAttributes({ { AttributeId::POSITION, "i_position" },
        { AttributeId::COLOR, "i_color" } });
    program.SetUniforms({ { UniformId::MATRIX_PROJECTION, "u_projection_matrix" } });

    program.CompileShader(GL_VERTEX_SHADER, kVertexShaderCode);
    program.CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode);
    program.Link();

    program.Use();

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

    UploadVertexData(program, vertexBufferData, std::size(vertexBufferData));

    glm::mat4 projMatrix = MakeOrthoMat4(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
    auto projMatrixVar = program.GetUniform(UniformId::MATRIX_PROJECTION);
    projMatrixVar.SetValue(projMatrix);

    sdl::EventLoop loop;
    loop.DoOnDraw([&] {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    });

    loop.Run(*window);
}

int main(int /*argc*/, char* /*argv*/ [])
{
    try
    {
        RunApp();
    }
    catch (const std::exception& ex)
    {
        sdl::ShowErrorBox(ex.what());
    }

    return 0;
}
