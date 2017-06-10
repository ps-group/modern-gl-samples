#include "MouseGrabber.h"
#include <glm/vec2.hpp>

namespace sdl
{

MouseGrabber::MouseGrabber(SDL_Window& window)
    : m_windowRef(window)
{
    // Включаем режим спрятанного курсора.
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

bool MouseGrabber::OnMouseMove(const SDL_MouseMotionEvent &event)
{
    const glm::ivec2 delta = { event.xrel, event.yrel };
    bool filtered = false;

    // Проверяем, является ли событие автосгенерированным
    //  из-за предыдущего программного перемещения мыши.
    auto it = std::find(m_blacklist.begin(), m_blacklist.end(), delta);
    if (it != m_blacklist.end())
    {
        m_blacklist.erase(it);
        filtered = true;
    }
    else
    {
        WarpMouse();
    }

    return filtered;
}

void MouseGrabber::WarpMouse()
{
    // Помещаем курсор в центр.
    const glm::ivec2 windowCenter = GetWindowSize(m_windowRef) / 2;
    SDL_WarpMouseInWindow(&m_windowRef, windowCenter.x, windowCenter.y);

    // После перемещения возникает событие перемещения мыши.
    // Мы можем предсказать параметры события и добавить их в чёрный список.
    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    const glm::ivec2 reflectedPos = windowCenter - mousePos;
    m_blacklist.push_back(reflectedPos);
}

} // namespace sdl
