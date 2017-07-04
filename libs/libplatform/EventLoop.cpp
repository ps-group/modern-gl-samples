#include "EventLoop.h"
#include "ps_sdl.h"

namespace ps
{

void EventLoop::DoOnUpdate(const UpdateHandler &onUpdate)
{
    m_onUpdate = onUpdate;
}

void EventLoop::DoOnDraw(const VoidHandler &onDraw)
{
    m_onDraw = onDraw;
}

void EventLoop::DoOnEvent(SDL_EventType type, const EventHander &handler)
{
    m_eventHandlers[type] = handler;
}

void EventLoop::SetFramesPerSecond(unsigned fps)
{
    assert(fps > 0 && fps <= 60); // FPS over 60 can be ignored due to vsync.
    m_framePeriod = 1.0 / static_cast<double>(fps);
}

void EventLoop::Run(SDL_Window &window)
{
    detail::Timer swapTimer;
    detail::Timer updateTimer;

    for (;;)
    {
        // Invoke event handlers.
        SDL_Event event = { 0 };
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                return;
            }
            auto it = m_eventHandlers.find(static_cast<SDL_EventType>(event.type));
            if (it != m_eventHandlers.end())
            {
                it->second(event);
            }
        }

        // Invoke update callback.
        if (m_onUpdate)
        {
            m_onUpdate(updateTimer.Restart());
        }

        // Invoke draw callback.
        if (m_onDraw)
        {
            m_onDraw();
        }

        // Swap buffers, then wait for next frame time.
        SDL_GL_SwapWindow(&window);
        double sleepTime = swapTimer.Restart() - m_framePeriod;
        swapTimer.SleepFor(sleepTime);
    }
}

void EventLoop::PostQuitMessage()
{
    // See also https://wiki.libsdl.org/SDL_PushEvent
    SDL_Event event = { 0 };
    event.type = SDL_QUIT;
    detail::ThrowIf(SDL_PushEvent(&event) < 0);
}

}
