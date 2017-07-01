#pragma once
#include "detail/ps_sdl_detail.h"
#include "type_aliases.h"
#include <SDL2/SDL.h>
#include <glm/fwd.hpp>
#include <memory>
#include <string_view>
#include <type_traits>

namespace ps
{
// Используем unique_ptr с явно заданным функтором удаления вместо delete.
using SDLWindowPtr = std::unique_ptr<SDL_Window, detail::SDLWindowDeleter>;
using SDLGLContextPtr = std::unique_ptr<void, detail::SDLGLContextDeleter>;
using SDLSurfacePtr = std::unique_ptr<SDL_Surface, detail::SDLSurfaceDeleter>;
using TTFFontPtr = std::unique_ptr<TTF_Font, detail::TtfFontDeleter>;
using SDLString = std::unique_ptr<char, detail::SDLStringDeleter>;

// Класс инициализирует следующие модули SDL2:
// - ядро SDL2
// - модуль image
// - модуль TTF
class InitSDL
{
public:
    InitSDL();
    ~InitSDL();

private:
    detail::InitSDLCore m_core;
    detail::InitSDLImage m_image;
    detail::InitSDLTtf m_ttf;
};

struct GraphicsAttributes
{
    int versionMajor = 3;
    int versionMinor = 3;
    bool debug = false;
    bool multisampling = true;
};

// Функция должна быть вызвана до создания окна,
//  иначе атрибуты могут быть проигнорированы.
void SetGraphicsAttributes(const GraphicsAttributes &attrs);

// Показывает message box с ошибкой.
void ShowErrorBox(std::string_view text);

// Создаёт центрированное окно с заданным заголовком, размером и флагами.
// Набор допустимых флагов определён в SDL_WindowFlags.
SDLWindowPtr CreateWindow(std::string_view title, const glm::uvec2& size, unsigned flags = 0);

// Создаёт полноэкранное окно с заданными флагами.
// Набор допустимых флагов определён в SDL_WindowFlags.
SDLWindowPtr CreateFullscreenWindow(unsigned flags = 0);

// Возвращает текущий размер окна
glm::ivec2 GetWindowSize(SDL_Window& window);

// Разворачивает окно на всю свободную область экрана
void MaximizeWindow(SDL_Window &window);

// Делает окно полноэкранным либо не полноэкранным
void SetWindowFullscreen(SDL_Window &window, bool fullscreen);

// Устанавливает заголовок окна
void SetWindowTitle(SDL_Window& window, std::string_view title);

// Creates Core Profile OpenGL context for given window.
SDLGLContextPtr CreateGraphicsContext(SDL_Window& window);

// Возвращает путь к каталогу, в котором находится текущий исполняемый файл.
fs::path GetBasePath();

// Возвращает путь к каталогу, в котором можно сохранять файлы настроек.
// Каждая ОС имеет свой стандарт выбора каталога настроек, функция учитывает это.
fs::path GetPrefPath(std::string_view vendorName, std::string_view appName);

// Конвертирует цвет из vec4 со значениями от 0 до 1 в структуру RGBA цвета.
SDL_Color ConvertColor(const glm::vec4& rgba);

// Загружает изображение в формате PNG, JPEG, BMP, ICO, TIFF или WEBP из файла
SDLSurfacePtr ImageLoad(const fs::path& path);

// Изменяет изображение, зеркально отражая пиксели по оси Y.
void FlipSurfaceY(SDL_Surface& surface);

// Открывает файл шрифта и возвращает объект шрифта
// @param ptsize - размер в точках (pt).
TTFFontPtr OpenFont(const fs::path& path, int ptsize);

// Рендерит в текстуру текст в кодировке UTF-8 с заданным RGBA цветом.
SDLSurfacePtr RenderUtf8(TTF_Font& font, std::string_view text, const glm::vec4& rgba);
}
