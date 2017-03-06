#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Window.hpp"


namespace cppgui {

    Window::Window(const char * title):
        SDL2_window(title)
    {
        _root_widget.on_invalidated([this]() { invalidate(); });
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

    void Window::redraw(void * context)
    {
        _root_widget.render(_canvas.get(), {0, 0});
    }

    void Window::size_changed(int w, int h) // TODO: use Extents ?
    {
        // (Re-)do layout
        _root_widget.init_layout();
        _root_widget.set_bounds({0, 0}, cppgui::Bounding_box{cppgui::Extents{w, h}});
    }

} // ns cppgui