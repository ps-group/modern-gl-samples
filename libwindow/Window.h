#pragma once
#include <glm/vec2.hpp>

// Событие, связанное с виртуальным касанием.
struct TouchEvent
{
    // Точное положение мыши.
    glm::vec2 position;
    // Относительное смещение (x, y) от предыдущего положения мыши.
    glm::vec2 offset;
};

struct ResizeEvent
{
    // Предыдущий размер окна до изменения.
    glm::vec2 oldSize;
    // Размер окна после изменения.
    glm::vec2 newSize;
};

// Идентификаторы клавиш, заданные в контексте приложения.
enum class VirtualKeyId
{
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
};

class IWindowDelegate
{
public:
    virtual ~IWindowDelegate() = default;

    virtual void OnTouchStart(const TouchEvent &) {}
    virtual void OnTouchEnd(const TouchEvent &) {}
    virtual void OnTouchMove(const TouchEvent &) {}
    virtual void OnVirtualKeyDown(const VirtualKeyId &) {}
    virtual void OnVirtualKeyUp(const VirtualKeyId &) {}
    virtual void OnResize() {}

    virtual void OnUpdate() {}
    virtual void OnDraw() {}
};

class CWindow
{
public:
};
