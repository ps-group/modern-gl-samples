#include "ps_sdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

using namespace glm;

namespace ps
{
InitSDL::InitSDL()
{
}

InitSDL::~InitSDL()
{
}

void SetGraphicsAttributes(const GraphicsAttributes& attrs)
{
    // Допустимы только версии OpenGL 3.x и выше
    assert(attrs.versionMajor >= 3);
    assert(attrs.versionMinor >= 0);

    // Выбираем версию контекста
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, attrs.versionMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, attrs.versionMinor);

    // Включаем отказ от расширений совместимости.
    int flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    if (attrs.debug)
    {
        // Если указано, включаем отладочный флаг
        flags = flags | SDL_GL_CONTEXT_DEBUG_FLAG;
    }

    // Выбираем параметры совместимости контекста
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags);

    // Включаем двойную буферизацию
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Включаем режим сглаживания с помощью субпиксельного рендеринга.
    if (attrs.multisampling)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }
}

void ShowErrorBox(std::string_view text)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!",
                             std::string(text).c_str(), nullptr);
}

SDLWindowPtr CreateWindow(std::string_view title, const glm::uvec2& size, unsigned flags)
{
    // Force OpenGL support.
    flags |= SDL_WINDOW_OPENGL;

    // See also https://wiki.libsdl.org/SDL_CreateWindow
    auto* window = SDL_CreateWindow(
        std::string(title).data(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(size.x),
        static_cast<int>(size.y),
        flags);

    detail::ThrowIf(window == nullptr);
    return SDLWindowPtr(window);
}

SDLWindowPtr CreateFullscreenWindow(unsigned flags)
{
    // Force fullscreen and OpenGL support.
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL;

    // See also https://wiki.libsdl.org/SDL_GetDisplayBounds
    SDL_Rect rect = { 0, 0, 0, 0 };
    detail::ThrowIf(SDL_GetDisplayBounds(0, &rect) != 0);

    auto* window = SDL_CreateWindow(
        "", // title useless in fullscreen.
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        rect.w,
        rect.h,
        flags);
    detail::ThrowIf(window == nullptr);
    return SDLWindowPtr(window);
}

ivec2 GetWindowSize(SDL_Window& window)
{
    ivec2 result;
    SDL_GetWindowSize(&window, &result.x, &result.y);
    return result;
}

void MaximizeWindow(SDL_Window &window)
{
    SDL_MaximizeWindow(&window);
}

void SetWindowFullscreen(SDL_Window &window, bool fullscreen)
{
    SDL_SetWindowFullscreen(&window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void SetWindowTitle(SDL_Window &window, std::string_view title)
{
    SDL_SetWindowTitle(&window, std::string(title).c_str());
}

SDLGLContextPtr CreateGraphicsContext(SDL_Window& window)
{
    // See also https://wiki.libsdl.org/SDL_GL_CreateContext
    auto* context = SDL_GL_CreateContext(&window);
    detail::ThrowIf(context == nullptr);
    return SDLGLContextPtr(context);
}

fs::path GetBasePath()
{
    auto pathUtf8 = SDLString(SDL_GetBasePath());
    return fs::u8path(pathUtf8.get());
}

fs::path GetPrefPath(std::string_view vendorName, std::string_view appName)
{
    auto pathUtf8 = SDLString(SDL_GetPrefPath(vendorName.data(), appName.data()));
    return fs::u8path(pathUtf8.get());
}

SDL_Color ConvertColor(const glm::vec4& rgba)
{
    const vec4 scaledColor = 255.f * glm::clamp(rgba, vec4(0.f), vec4(1.f));
    SDL_Color result;
    result.r = Uint8(scaledColor.r);
    result.g = Uint8(scaledColor.g);
    result.b = Uint8(scaledColor.b);
    result.a = Uint8(scaledColor.a);

    return result;
}

SDLSurfacePtr ImageLoad(const fs::path &path)
{
    auto* surface = IMG_Load(path.u8string().c_str());
    detail::ThrowIf(surface == nullptr);

    return SDLSurfacePtr(surface);
}

void FlipSurfaceY(SDL_Surface& surface)
{
    const size_t rowSize = size_t(surface.w * surface.format->BytesPerPixel);
    std::vector<uint8_t> row(rowSize);

    // Зеркально отражаем пиксели по оси Y,
    //  если число строк пикселей в изображении нечётное,
    //  центральная строка остаётся нетронутой.
    for (size_t y = 0, height = size_t(surface.h); y < height / 2; ++y)
    {
        auto* pixels = reinterpret_cast<uint8_t*>(surface.pixels);
        auto* upperRow = pixels + rowSize * y;
        auto* lowerRow = pixels + rowSize * (height - y - 1);
        std::memcpy(row.data(), upperRow, rowSize);
        std::memcpy(upperRow, lowerRow, rowSize);
        std::memcpy(lowerRow, row.data(), rowSize);
    }
}

TTFFontPtr OpenFont(const fs::path& path, int ptsize)
{
    auto* font = TTF_OpenFont(path.u8string().c_str(), ptsize);
    detail::ThrowIf(font == nullptr);

    return TTFFontPtr(font);
}

SDLSurfacePtr RenderUtf8(TTF_Font& font, std::string_view text, const vec4& rgba)
{
    SDL_Color color = ConvertColor(rgba);
    auto* surface = TTF_RenderUTF8_Blended(&font, std::string(text).data(), color);
    detail::ThrowIf(surface == nullptr);
    return SDLSurfacePtr(surface);
}

}
