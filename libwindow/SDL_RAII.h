#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <type_traits>

// Предварительное объявление структур из SDL2,
//  позволяет не включать заголовки заранее.
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Surface SDL_Surface;
typedef void* SDL_GLContext;
typedef struct _TTF_Font TTF_Font;

namespace detail
{
struct SDLWindowDeleter
{
    void operator()(SDL_Window* ptr);
};
struct SDLGLContextDeleter
{
    void operator()(SDL_GLContext ptr);
};
struct SDLSurfaceDeleter
{
    void operator()(SDL_Surface* ptr);
};
struct TtfFontDeleter
{
    void operator()(TTF_Font* font);
};
}

// Используем unique_ptr с явно заданным функтором удаления вместо delete.
using SDLWindowPtr = std::unique_ptr<SDL_Window, detail::SDLWindowDeleter>;
using SDLGLContextPtr = std::unique_ptr<std::decay_t<SDL_GLContext>, detail::SDLGLContextDeleter>;
using SDLSurfacePtr = std::unique_ptr<SDL_Surface, detail::SDLSurfaceDeleter>;
using TTFFontPtr = std::unique_ptr<TTF_Font, detail::TtfFontDeleter>;
