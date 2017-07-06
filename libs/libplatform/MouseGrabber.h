#pragma once
#include <vector>

#if 0

// TODO: <sergey.shambir> портировать на SFML, см. setMouseCursorGrabbed

namespace ps
{

// Класс захватывает курсор мыши, а также решает проблему
//  сохранения невидимого курсора в центре окна:
//  - на каждом кадре курсор принудительно перемещается в центр окна
//  - перемещение курсора порождает событие mouse moved
//    с известными deltaX, deltaY, что позволяет
//    игнорировать это событие.
// Можно заподозрить, что иногда mouse grabber будет фильтровать
//  не те события и тем самым нарушать правильную обработку мыши,
//  но это неправда: общее движение мыши остаётся инвариантом.
class MouseGrabber
{
public:
    MouseGrabber(SDL_Window& window);

    // Возвращает true, если событие было поглощено.
    // Событие поглощается, если оно порождено программным
    //  перемещением мыши.
    bool OnMouseMove(const SDL_MouseMotionEvent& event);

private:
    void WarpMouse();

    SDL_Window& m_windowRef;

    // Ожидаемые координаты событий mouse move, порождаемых
    //  программным перемещением курсора,
    //  события с такими координатами игнорируются.
    std::vector<glm::ivec2> m_blacklist;
};

} // namespace ps

#endif
