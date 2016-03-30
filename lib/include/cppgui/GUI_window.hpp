#pragma once

#include "./Canvas.hpp"

namespace cppgui {

    /** The class template is intended as a convenience: it augments the specified
        implementation of the Window concept with a fully connected root widget.
     */
    template <
        class GUIConfig,                            // GUI library configuration
        class WindowBaseT,                          // Window "concept" implementation
        template <class> class RendererAdapter      // "glue" code
    >
    class GUI_window: public WindowBaseT, 
        public RendererAdapter<GUI_window<GUIConfig, WindowBaseT, RendererAdapter>>
    {

    public:
        using Canvas_t = typename Canvas<typename GUIConfig::Renderer>;
        using Root_widget = cppgui::Root_widget<GUIConfig, true>;
        using Keycode = typename GUIConfig::Keyboard::Keycode;

        GUI_window(const char *title, int w = 800, int h = 600); // TODO: better defaults

        auto& root_widget() { return _root_widget; }

    protected:

        // Concept implementation (CRTP)

        void init_graphics();
        void cleanup_graphics();

        void redraw();

        void size_changed(int w, int h);
        void mouse_motion(int x, int y);
        void mouse_button(int x, int y, int button, int dir, int count);
        void mouse_wheel(int x, int y);
        void text_input(const char32_t *, size_t);
        void key_down(const Keycode &);

        void closing();

    private:
        Canvas_t                   *_canvas;        // TODO: use unique_ptr<>
        Root_widget                 _root_widget;
    };

} // ns cppgui
