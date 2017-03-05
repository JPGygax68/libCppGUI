#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Window.hpp"


namespace cppgui {

    Window::Window(const char * title):
        SDL2_window(title)
    {
    }

    void Window::init_graphics(void *context)
    {
        _canvas = std::make_unique<Canvas>();
        _canvas->init();
    }

    void Window::cleanup_graphics(void *context)
    {
        _canvas->cleanup();
        _canvas.release();
    }

} // ns cppgui