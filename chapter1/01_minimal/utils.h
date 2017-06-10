#pragma once

#include <SDL2/SDL.h>
#include <glbinding/gl/types.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <string_view>
#include <vector>

enum AttributeId
{
    attrib_position,
    attrib_color
};

using AttributeBinding = std::vector<std::pair<AttributeId, std::string_view>>;

struct Vertex
{
    glm::vec4 rgba;
    glm::vec2 xy;
};

SDL_Window* CreateWindow(std::string_view title, int width, int height);
void PrintOpenGLVersion();
gl::GLuint CompileShader(gl::GLenum type, std::string_view sourceCode);
void DoEventLoop(SDL_Window* window, const std::function<void()>& draw);
gl::GLuint LinkProgram(gl::GLuint vs, gl::GLuint fs, const AttributeBinding& binding);
glm::mat4 MakeOrthoMat4(float left, float right, float bottom, float top, float znear, float zfar);
gl::GLuint MakeVertexArray();
gl::GLuint MakeBuffer();
void UploadVertexData(const Vertex* data, size_t count);
