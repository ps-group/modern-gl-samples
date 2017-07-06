#include "EventLoop.h"
#include <SFML/Window.hpp>
#include <thread>

namespace ps
{

void EventLoop::DoOnUpdate(const UpdateHandler& onUpdate)
{
	m_onUpdate = onUpdate;
}

void EventLoop::DoOnDraw(const DrawHandler& onDraw)
{
	m_onDraw = onDraw;
}

void EventLoop::DoOnEvent(sf::Event::EventType type, const EventHander& handler)
{
	m_eventHandlers[type] = handler;
}

void EventLoop::SetFramesPerSecond(unsigned fps)
{
	assert(fps > 0 && fps <= 60); // FPS выше 60 будет наверняка проигнорирован из-за vsync.
	m_framePeriod = fp_seconds(1.0 / static_cast<double>(fps));
}

void EventLoop::Run(sf::Window& window)
{
	Timer<fp_seconds> swapTimer;
	Timer<fp_seconds> updateTimer;

	while (window.isOpen() && !m_willQuit)
	{
		// Обработка событий с вызовом пользовательских обработчиков.
		sf::Event event = { sf::Event::Closed, 0 };
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			auto it = m_eventHandlers.find(event.type);
			if (it != m_eventHandlers.end())
			{
				it->second(event);
			}
		}

		// Обратный вызов для обновления состояния.
		if (m_onUpdate)
		{
			m_onUpdate(updateTimer.Restart().count());
		}

		// Обратный вызов для рисования обновлённого состояния.
		if (m_onDraw)
		{
			m_onDraw(window);
		}

		// Вывод кадра на экран
		window.display();

		// Принудительное ожидание времени до следующего кадра.
		//const fp_seconds sleepTime = (swapTimer.Restart() - m_framePeriod);
		//if (sleepTime > fp_seconds::zero())
		//{
		//	std::this_thread::sleep_for(sleepTime);
		//}
	}
}

void EventLoop::DeferQuit()
{
	m_willQuit = true;
}
}
