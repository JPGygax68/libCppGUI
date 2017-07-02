#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Window.hpp"


namespace cppgui 
{

    // Main class -------------------------------------------------------------

    Window::Window(const std::string &title):
        SDL2_window{title},
        _root_widget{this}
    {
    }

    void Window::add_popup(ISurface *popup)
    {
        _popups.push_back(static_cast<Popup_base*>(popup));
        invalidate();
    }

    void Window::remove_popup(ISurface *popup)
    {
        _popups.remove(static_cast<Popup_base*>(popup));
        invalidate();
    }

    void Window::init_window(void *context)
    {
        create_ui();

        _root_widget.init_layout();

        _canvas = std::make_unique<Canvas>();
        _canvas->init();
        _root_widget.get_backend_resources(_canvas.get());
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
        before_draw_ui(context);
        draw_ui(context);
        done_rendering();
    }

    void Window::render(Canvas *canvas)
    {
        _root_widget.render(canvas, {0, 0});
        
        for (auto popup: _popups) popup->render(canvas);
    }

    void Window::draw_ui(void *context)
    {
        //_canvas->clear(_bkg_color);
        _canvas->enter_context(); // TODO: pass/check context ?
                                  // TODO: must be made optional!

        render(_canvas.get());

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

    
    // Inner class Popup_base -------------------------------------------------

    Window::Popup_base::Popup_base(ISurface *owner, const Rectangle &r):
        _owner{static_cast<Window*>(owner)},
        _rect{r}
    {
        _owner->add_popup(this);
    }

    Window::Popup_base::~Popup_base()
    {
        _owner->remove_popup(this);
    }

    auto Window::Popup_base::rectangle() -> Rectangle
    {
        return _rect;
    }

    void Window::Popup_base::invalidate()
    {
        // TODO: optimize
        _owner->invalidate();
    }

    void Window::Popup_base::add_popup(ISurface *popup)
    {
        _owner->add_popup(popup);
    }

    void Window::Popup_base::remove_popup(ISurface *popup)
    {
        _owner->remove_popup(popup);
    }

} // ns cppgui