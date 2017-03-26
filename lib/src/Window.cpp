#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Window.hpp"


namespace cppgui {

    Window::Window(const std::string &title):
        SDL2_window(title),
        _bkg_color{0, 0, 0, 0}
    {
        _root_widget.on_invalidated([this]() { invalidate(); });
    }

    void Window::set_background_color(const RGBA &color)
    {
        _bkg_color = color;
    }

    void Window::init_window(void *context)
    {
        create_ui();

        _root_widget.init_layout();

        _canvas = std::make_unique<Canvas>();
        _canvas->init();
        _root_widget.init(_canvas.get());
    }

    void Window::cleanup_window(void *context)
    {
        _root_widget.cleanup(); // TODO: pass canvas ?
        _canvas->cleanup();
        _canvas.release();
    }

    void Window::redraw(void *context)
    {
        begin_rendering();
        before_redraw(context);
        draw_ui();
        done_rendering();
    }

    void Window::draw_ui()
    {
        //_canvas->clear(_bkg_color);
        _canvas->enter_context(); // TODO: pass/check context ?
                                  // TODO: must be made optional!
        // TODO: pass context to canvas / check if matches / etc ?
        before_draw_ui(_canvas.get());
        _root_widget.render(_canvas.get(), {0, 0});
        _canvas->leave_context();
    }

    void Window::size_changed(const Extents &e)
    {
        // Inform canvas
        _canvas->define_viewport(0, 0, e.w, e.h);
        // (Re-)do layout
        _root_widget.init_layout();
        _root_widget.set_bounds({0, 0}, Bbox{e, left, top});
        adjust_layout();
        _root_widget.compute_view_from_data();
    }

    void Window::text_input(const char32_t *text, size_t cp_count)
    {
        _root_widget.text_input(text, cp_count);
    }

    void Window::key_down(Keycode key)
    {
        _root_widget.key_down(key);
    }

    void Window::mouse_motion(const Point &p)
    {
        _root_widget.mouse_motion(p);
    }

    void Window::mouse_button(const Point &p, uint btn, Key_state state, uint clicks)
    {
        _root_widget.mouse_button(p, btn, state, clicks);
    }

    void Window::mouse_wheel(const Point &p)
    {
        _root_widget.mouse_wheel(p);
    }

} // ns cppgui