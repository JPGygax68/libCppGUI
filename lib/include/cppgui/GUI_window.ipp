#include <iostream> // TODO: for debugging only, remove!

#include "./GUI_window.hpp"

namespace cppgui {


    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::GUI_window(const char *title, int w, int h):
        WindowBaseT(title, w, h)
    {
        _root_widget.on_invalidated([this]() { invalidate(); });
    }

    /** Caution! the following is called from the constructor of the concrete class derived from
        GUI_window<>.

        TODO: better name for this method ?
    */
    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::init_window()
    {
        WindowBaseT::init_window();

        _canvas = new Canvas_t {}; // TODO: support passing window as parameter
        _canvas->init();

        _root_widget.set_canvas(_canvas);

        _root_widget.init();
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::cleanup_window()
    {
        assert(_canvas);
        _canvas->cleanup();
        delete _canvas;
        _canvas = nullptr;

        WindowBaseT::cleanup_window();
    }

    /*
    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::init_gui()
    {
        if (!_init_done)
        {
            _root_widget.init();
            _init_done = true;
        }
    }
    */

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::redraw()
    {
        // std::cout << "Test_window::redraw()" << std::endl;

        _canvas->enter_context();
        //_root_widget.render(_canvas, { 0, 0 });
        _root_widget.render();
        _canvas->leave_context();
        
        present();
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::size_changed(int w, int h)
    {
        _root_widget.set_extents({ (unsigned)w, (unsigned)h });
        _canvas->define_viewport(0, 0, w, h);
        _root_widget.layout();
        //init_gui();
        _root_widget.compute_view_from_data();
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_motion(int x, int y)
    {
        _root_widget.mouse_motion({x, y});
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_button(int x, int y, int button, int dir, int count)
    {
        _root_widget.mouse_button({ x, y }, button, dir == down ? cppgui::pressed : cppgui::released);

        if (dir == up)
        {
            _root_widget.mouse_click({ x, y, }, button, count);
        }
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::mouse_wheel(int x, int y)
    {
        _root_widget.mouse_wheel({x, y});
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::text_input(const char32_t *text, size_t size)
    {
        _root_widget.text_input(text, size);
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::key_down(const Keycode &key)
    {
        _root_widget.key_down(key);
    }

    template<class Impl, class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    void GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter>::closing()
    {
        p()->cleanup_window();
    }

} // ns cppgui