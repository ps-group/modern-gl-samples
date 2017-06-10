#pragma once
#include <functional>
#include <unordered_map>
#include <SDL2/SDL.h>

namespace sdl
{

using UpdateHandler = std::function<void(double secondsElapsed)>;
using VoidHandler = std::function<void()>;
using EventHander = std::function<void(const SDL_Event& event)>;

// Класс реализует паттерн проектирования игр Game Loop.
// Такой паттерн подходит не только для игр, но и для
//  других применений OpenGL.
// См. https://martalex.gitbooks.io/gameprogrammingpatterns/content/chapter-3/3.2-game-loop.html
class EventLoop
{
public:
    // Устанавливает функцию, вызываемую для обновления состояния сцены.
    // Функция вызывается на каждом кадре и получает дробное число секунд,
    //  прошедших с предыдущего вызова.
    void DoOnUpdate(const UpdateHandler& onUpdate);

    // Устанавливает функцию, вызываемую для рисования сцены.
    // В идеале такая функция не должна менять состояние сцены.
    void DoOnDraw(const VoidHandler& onDraw);

    // Устанавливает фукнцию, вызываемую при определённом типе событий.
    void DoOnEvent(SDL_EventType type, const EventHander& handler);

    // Устанавливает верхнее ограничение числа кадров в секунду,
    //  допустимо указывать fps от 1 до 60.
    void SetFramesPerSecond(unsigned fps);

    // Запускает цикл событий, используя заданное окно для рисования.
    void Run(SDL_Window& window);

    // Добавляет событие, которое при обработке вызовет выход из цикла событий.
    // Фактически выход состоится после окончания текущего кадра.
    void PostQuitMessage();

private:
    double m_framePeriod = 1.0 / 60.0;
    UpdateHandler m_onUpdate;
    VoidHandler m_onDraw;
    std::unordered_map<SDL_EventType, EventHander> m_eventHandlers;
};

}
