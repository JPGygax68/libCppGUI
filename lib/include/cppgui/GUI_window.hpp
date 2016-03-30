#pragma once

#include "./Canvas.hpp"

namespace cppgui {

    template <class Config, class WindowBaseT, template <class> class RendererAdapter>
    class GUI_window: public WindowBaseT, 
        public RendererAdapter<GUI_window<Config, WindowBaseT, RendererAdapter>>
    {

    public:
        using Canvas_t = typename Canvas<typename Config::Renderer>;
        using Root_widget = cppgui::Root_widget<Config, true>;
        using Keycode = typename Config::Keyboard::Keycode;

        GUI_window(const char *title, int w = 800, int h = 600); // TODO: better defaults

        auto& root_widget() { return _root_widget; }

        // Concept implementation

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
