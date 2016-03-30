#include <iostream> // TODO: for debugging only, remove!

#include "./GUI_window.hpp"

namespace cppgui {


    template<class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::GUI_window(const char *title, int w, int h):
        WindowBaseT(title, w, h),
        _root_widget{ _canvas }
    {
    }

    /** Caution! the following is called from the constructor of Window, i.e. *before* the body
    of the constructor of Test_window (which is derived from Window).
    */
    template<class GUIConfig, class WindowBaseT, template <class> class RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::init_graphics()
    {
        std::cout << "Test_window::init()" << std::endl;

        init_renderer();

        _canvas = new Canvas_t{}; // TODO: support passing window as parameter
        _canvas->init();
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::cleanup_graphics()
    {
        assert(_canvas);
        delete _canvas;
        _canvas = nullptr;

        cleanup_renderer();
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::redraw()
    {
        std::cout << "Test_window::redraw()" << std::endl;

        //GL(Clear, GL_COLOR_BUFFER_BIT);

        _canvas->enter_context();
        _root_widget.render(_canvas, { 0, 0 });
        _canvas->leave_context();
        
        present();
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::size_changed(int w, int h)
    {
        _root_widget.set_extents({ (unsigned)w, (unsigned)h });
        _root_widget.layout();
        _canvas->define_viewport(0, 0, w, h);
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::mouse_motion(int x, int y)
    {
        _root_widget.mouse_motion({x, y});
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::mouse_button(int x, int y, int button, int dir, int count)
    {
        _root_widget.mouse_button({ x, y }, button, dir == down ? cppgui::pressed : cppgui::released);

        if (dir == up)
        {
            _root_widget.mouse_click({ x, y, }, button, count);
        }
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::mouse_wheel(int x, int y)
    {
        _root_widget.mouse_wheel({x, y});
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::text_input(const char32_t *text, size_t size)
    {
        _root_widget.text_input(text, size);
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::key_down(const Keycode &key)
    {
        _root_widget.key_down(key);
    }

    template<class GUIConfig, class WindowBaseT, template <class> class  RendererAdapter>
    inline void GUI_window<GUIConfig, WindowBaseT, RendererAdapter>::closing()
    {
        cleanup_graphics();
    }

} // ns cppgui