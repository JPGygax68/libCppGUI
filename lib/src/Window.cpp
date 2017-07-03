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
        auto size = popup->rectangle().ext;
        popup->size_changed(size);
            // ^ slightly odd - the popup is being told its own size
        invalidate();
    }

    void Window::remove_popup(ISurface *popup)
    {
        _popups.remove(static_cast<Popup_base*>(popup));
        invalidate();
    }

    void Window::init(void *context)
    {
        create_ui();

        _root_widget.init_layout();

        _canvas = std::make_unique<Canvas>();
        _canvas->init();
        canvas_created(_canvas.get());
    }

    void Window::cleanup(void *context)
    {
        before_destroy_canvas(_canvas.get());
        _root_widget.cleanup(); // TODO: pass canvas ?
        _canvas->cleanup();
        _canvas.release();
    }

    void Window::redraw(void *context)
    {
        begin_rendering();
        before_draw_ui(context);
        render_canvas(context);
        done_rendering();
    }

    void Window::render_ui(Canvas *canvas)
    {
        _root_widget.render(canvas, {0, 0});
        
        for (auto popup: _popups) popup->render_ui(canvas);
    }

    void Window::render_canvas(void *context)
    {
        //_canvas->clear(_bkg_color);
        _canvas->enter_context(); // TODO: pass/check context ?
                                  // TODO: must be made optional!

        render_ui(_canvas.get());

        _canvas->leave_context();
    }

    void Window::size_changed(const Extents &e)
    {
        // Inform canvas
        _canvas->define_viewport(0, 0, e.w, e.h);
        // (Re-)do layout
        _root_widget.init_layout(); // TODO: should not be called more than once
        _root_widget.set_bounds({0, 0}, Bbox{e, left, top});
        adjust_layout();
        _root_widget.compute_view_from_data();  // TODO: clarify if this may be called more than once
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
        // TODO: popups
        _root_widget.mouse_motion(p);
    }

    void Window::mouse_button(const Point &p, uint btn, Key_state state, uint clicks)
    {
        // TODO: popups
        _root_widget.mouse_button(p, btn, state, clicks);
    }

    void Window::mouse_wheel(const Point &p)
    {
        // TODO: popups
        _root_widget.mouse_wheel(p);
    }

    void Window::canvas_created(Canvas *canvas)
    {
        _root_widget.get_backend_resources(canvas);
    }

    void Window::before_destroy_canvas(Canvas *canvas)
    {
        //_root_widget.release_backend_resources(_canvas.get());
    }


    // Inner class Popup_base -------------------------------------------------

    Window::Popup_base::Popup_base(ISurface *owner, const Rectangle &r):
        _owner{static_cast<Window*>(owner)},
        _rect{r}
    {
    }

    Window::Popup_base::~Popup_base()
    {
        _owner->remove_popup(this);
    }

    void Window::Popup_base::show()
    {
        _owner->add_popup(this);
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
        // Since a popup cannot own a popup, we delegate to owning window
        _owner->add_popup(popup);
    }

    void Window::Popup_base::remove_popup(ISurface *popup)
    {
        _owner->remove_popup(popup);
    }

} // ns cppgui