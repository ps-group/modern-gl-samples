#pragma once

// Предварительное объявление структур из SDL2,
//  позволяет не включать заголовки заранее.
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Surface SDL_Surface;
typedef void* SDL_GLContext;
typedef struct _TTF_Font TTF_Font;

namespace sdl::detail
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
    void operator()(TTF_Font* ptr);
};
struct SDLStringDeleter
{
    void operator()(char* ptr);
};

class InitSDLCore
{
public:
    InitSDLCore();
    ~InitSDLCore();
};

class InitSDLImage
{
public:
    InitSDLImage();
    ~InitSDLImage();
};

class InitSDLTtf
{
public:
    InitSDLTtf();
    ~InitSDLTtf();
};

class Timer
{
public:
    Timer();

    // Возвращает число секунд с предыдущего вызова Restart.
    double Restart();

    // Если seconds > 0, засыпает на указанное дробное число секунд.
    void SleepFor(double seconds);

private:
    unsigned m_lastTimeMsec = 0;
};

void ThrowIf(bool error);
}
