#include "SDL_RAII.h"

namespace detail
{

void detail::SDLWindowDeleter::operator()(SDL_Window *ptr)
{
    SDL_DestroyWindow(ptr);
}

void SDLGLContextDeleter::operator()(SDL_GLContext ptr)
{
    SDL_GL_DeleteContext(ptr);
}

void SDLSurfaceDeleter::operator()(SDL_Surface *ptr)
{
    SDL_FreeSurface(ptr);
}

void TtfFontDeleter::operator()(TTF_Font *font)
{
    TTF_CloseFont(font);
}

}
