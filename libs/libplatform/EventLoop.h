#pragma once
#include "Timer.h"
#include <SFML/Window/Event.hpp>
#include <functional>
#include <unordered_map>

namespace ps
{

using UpdateHandler = std::function<void(float seconds)>;
using DrawHandler = std::function<void(sf::Window& window)>;
using EventHander = std::function<void(const sf::Event& event)>;

// Класс реализует паттерн проектирования игр Game Loop.
// Такой паттерн подходит не только для игр, но и для
//  других применений OpenGL.
// См. https://martalex.gitbooks.io/gameprogrammingpatterns/content/chapter-3/3.2-game-loop.html
class EventLoop
{
public:
	// Устанавливает функцию, вызываемую для обновления состояния сцены.
	// Функция вызывается на каждом кадре и получает дробное число секунд,
	//  прошедших с предыдущего обновления.
	void DoOnUpdate(const UpdateHandler& onUpdate);

	// Устанавливает функцию, вызываемую для рисования сцены.
	// В идеале такая функция не должна менять состояние сцены.
	void DoOnDraw(const DrawHandler& onDraw);

	// Устанавливает фукнцию, вызываемую при определённом типе событий.
	void DoOnEvent(sf::Event::EventType type, const EventHander& handler);

	// Устанавливает верхнее ограничение числа кадров в секунду,
	//  допустимо указывать fps от 1 до 60.
	void SetFramesPerSecond(unsigned fps);

	// Запускает цикл событий, используя заданное окно для рисования.
	void Run(sf::Window& window);

	// Планирует выход из цикла событий после окончания текущего кадра.
	void DeferQuit();

private:
	bool m_willQuit = false;
	fp_seconds m_framePeriod = fp_seconds(1.0 / 60.0);
	UpdateHandler m_onUpdate;
	DrawHandler m_onDraw;
	std::unordered_map<sf::Event::EventType, EventHander> m_eventHandlers;
};
}
