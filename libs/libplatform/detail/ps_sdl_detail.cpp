#include "ps_sdl_detail.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <string>

namespace ps::detail
{

void detail::SDLWindowDeleter::operator()(SDL_Window* ptr)
{
	SDL_DestroyWindow(ptr);
}

void SDLGLContextDeleter::operator()(SDL_GLContext ptr)
{
	SDL_GL_DeleteContext(ptr);
}

void SDLSurfaceDeleter::operator()(SDL_Surface* ptr)
{
	SDL_FreeSurface(ptr);
}

void TtfFontDeleter::operator()(TTF_Font* ptr)
{
	TTF_CloseFont(ptr);
}

void SDLStringDeleter::operator()(char* ptr)
{
	SDL_free(ptr);
}

void ThrowIf(bool error)
{
	if (error)
	{
		std::string message = SDL_GetError();
		throw std::runtime_error("SDL2 error: " + message);
	}
}

Timer::Timer()
{
	Restart();
}

double Timer::Restart()
{
	const unsigned timeMsec = SDL_GetTicks();

	// After 18 days running program ticks counter overflows.
	if (m_lastTimeMsec > timeMsec)
	{
		m_lastTimeMsec = timeMsec;
		return 0;
	}

	const double secondsElapsed = 0.001 * (timeMsec - m_lastTimeMsec);
	m_lastTimeMsec = timeMsec;

	return secondsElapsed;
}

void Timer::SleepFor(double seconds)
{
	if (seconds > 0)
	{
		const auto msec = static_cast<unsigned>(1000 * seconds);
		if (msec > 0)
		{
			SDL_Delay(msec);
		}
	}
}

InitSDLCore::InitSDLCore()
{
	ThrowIf(SDL_Init(SDL_INIT_EVERYTHING) != 0);
}

InitSDLCore::~InitSDLCore()
{
	SDL_Quit();
}

InitSDLImage::InitSDLImage()
{
	const int formats = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_TIF;
	ThrowIf(IMG_Init(formats) == 0);
}

InitSDLImage::~InitSDLImage()
{
	IMG_Quit();
}

InitSDLTtf::InitSDLTtf()
{
	ThrowIf(TTF_Init() != 0);
}

InitSDLTtf::~InitSDLTtf()
{
	TTF_Quit();
}
}
