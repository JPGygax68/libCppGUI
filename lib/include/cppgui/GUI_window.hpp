#pragma once

#include "./Root_widget.hpp"
#include "./Canvas.hpp"

namespace cppgui {

    /** The class template is intended as a convenience: it augments the specified
        implementation of the Window concept with a ready-made root widget.

        IMPORTANT: this template requires explicit instantiation. The best way
            of doing this is via a typedef, which would then a) be used as the 
            ancestor of your derived class, and b) explicitly instantiated
            wherever the derived class is implemented, e.g.:
                
           Header:

                using Specialized_GUI_window = cppgui::GUI_window<...>;
                class My_window: public Specialized_GUI_window { ... };

           Implementation:

                template Specialized_GUI_window;

                My_window::My_window(...) { ... }
                ...
     */
    template <
        class Impl,                                 // CRTP
        class GUIConfig,                            // GUI library configuration
        class WindowBaseT,                          // Window "concept" implementation
        template <class> class RendererAdapter      // "glue" code, using the "aspect" pattern
    >
    class GUI_window: public WindowBaseT, 
        public RendererAdapter< GUI_window<Impl, GUIConfig, WindowBaseT, RendererAdapter> >
    {
    public:
        using Canvas_t = typename Canvas<typename GUIConfig::Renderer>;
        using Root_widget = cppgui::Root_widget<GUIConfig, true>;
        using Keycode = typename GUIConfig::Keyboard::Keycode;

        GUI_window(const char *title, int w = 800, int h = 600); // TODO: better defaults

        auto root_widget() { return &_root_widget; }

    protected:

        void init_window();
        void cleanup_window();

        void redraw();

        void size_changed(int w, int h);
        void mouse_motion(int x, int y);
        void mouse_button(int x, int y, int button, int dir, int count);
        void mouse_wheel(int x, int y);
        void text_input(const char32_t *, size_t);
        void key_down(const Keycode &);

        void closing();

    private:
        class Impl_t: public Impl { friend class GUI_window; };
        auto p() { return static_cast<Impl_t*>(this); }

        Canvas_t                   *_canvas;        // TODO: use unique_ptr<>
        Root_widget                 _root_widget;
    };

} // ns cppgui
