#pragma once
#include <csignal>

// Макрос PS_BREAKPOINT вызывает останов в отладчике.
#if defined(_WIN32)
#define PS_BREAKPOINT        \
	if (IsDebuggerPresent()) \
	{                        \
		__debugbreak();      \
	}
#elif !defined(NDEBUG)
#define PS_BREAKPOINT std::raise(SIGINT);
#else
#define PS_BREAKPOINT
#endif

namespace ps
{
// Функция работает только с контекстами OpenGL, созданными с флагом Debug.
void SetupDebugOutput();
}
