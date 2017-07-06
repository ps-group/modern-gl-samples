#pragma once
#include <chrono>

namespace ps
{

// Синоним типа целочисленного временного интервала, представленного наносекундами.
using nanoseconds = std::chrono::nanoseconds;

// Синоним типа временного интервала с плавающей точкой, представленного секундами.
using fp_seconds = std::chrono::duration<double>;

// Таймер отслеживает интервалы времени между вызовами Restart.
template<class DurationT>
class Timer
{
public:
	Timer()
	{
		Restart();
	}

	// Перезапускает отсчёт времени в таймере.
	DurationT Restart()
	{
		const auto nextTimePoint = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::duration_cast<DurationT>(nextTimePoint - m_lastTimePoint);
		m_lastTimePoint = nextTimePoint;

		return elapsed;
	}

private:
	std::chrono::high_resolution_clock::time_point m_lastTimePoint;
};
}
