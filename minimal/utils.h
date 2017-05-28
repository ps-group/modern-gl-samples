#pragma once

#include <SDL2/SDL.h>
#include <glbinding/gl/types.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

enum t_attrib_id
{
    attrib_position,
    attrib_color
};

using AttrBinding = std::vector<std::pair<t_attrib_id, std::string_view>>;

struct Vertex
{
    glm::vec4 rgba;
    glm::vec2 xy;
};

void InitSDL();
SDL_Window* CreateWindow(std::string_view title, int width, int height);
gl::GLuint CompileShader(gl::GLenum type, std::string_view sourceCode);
void DoEventLoop(SDL_Window* window, const std::function<void()>& draw);
gl::GLuint LinkProgram(gl::GLuint vs, gl::GLuint fs, const AttrBinding& binding);
glm::mat4 MakeOrthoMat4(float left, float right, float bottom, float top, float znear, float zfar);
gl::GLuint MakeVertexArray();
gl::GLuint MakeBuffer();
void UploadVertexData(const Vertex* data, size_t count);
