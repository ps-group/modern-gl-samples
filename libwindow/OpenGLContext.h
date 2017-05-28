#pragma once
#include <memory>
#include "SDL_RAII.h"

// Профиль контекста OpenGL
enum class ContextProfile
{
    // Профиль с расширениями совместимости, любая версия OpenGL.
    Compatibility,
    // Профиль для приложения, способного работать на OpenGL 3.1 и выше
    RobustOpenGL_3_1,
    // Профиль для приложения, способного работать на OpenGL 3.2 и выше
    RobustOpenGL_3_2,
    // Профиль для приложения, способного работать на OpenGL 4.0 и выше
    RobustOpenGL_4_0,
};

class OpenGLContextFactory
{
public:
    // Методы для настройки создаваемого контекста,
    //  должны быть установлены до вызова окна
    void SetGLVersion(ContextProfile profile);
    void SetDebugEnabled(bool enabled);

    SDLGLContextPtr Create();

private:
    ContextProfile m_profile = ContextProfile::Compatibility;
    bool m_debugEnabled = false;
};
