#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER
#include "Canvas.hpp"
#include "Window.hpp"


namespace cppgui {
    
    void Window::init_graphics(void *context)
    {
        _canvas = std::make_unique<Canvas>();
    }

    void Window::cleanup_graphics(void *context)
    {
        _canvas.release();
    }

} // ns cppgui