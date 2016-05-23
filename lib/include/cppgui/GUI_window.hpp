#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

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

        GUI_window(const char *title, int w = 800, int h = 600):
            WindowBaseT(title, w, h)
        {
            _root_widget.on_invalidated([this]() { invalidate(); });
        }

        auto root_widget() { return &_root_widget; }

    protected:

        void init_window()
        {
            WindowBaseT::init_window();

            _canvas = new Canvas_t {}; // TODO: support passing window as parameter
            _canvas->init();

            _root_widget.set_canvas(_canvas);

            _root_widget.init();
        }
        void cleanup_window()
        {
            assert(_canvas);
            _canvas->cleanup();
            delete _canvas;
            _canvas = nullptr;

            WindowBaseT::cleanup_window();
        }

        //void init_gui();
        // void cleanup_gui();

        void redraw()
        {
            // std::cout << "Test_window::redraw()" << std::endl;

            _canvas->enter_context();
            //_root_widget.render(_canvas, { 0, 0 });
            _root_widget.render();
            _canvas->leave_context();

            present();
        }

        void size_changed(int w, int h)
        {
            _root_widget.set_extents({ (unsigned)w, (unsigned)h });
            _canvas->define_viewport(0, 0, w, h);
            _root_widget.layout();
            //init_gui();
            _root_widget.compute_view_from_data();
        }
        void mouse_motion(int x, int y)
        {
            _root_widget.mouse_motion({x, y});
        }
        void mouse_button(int x, int y, int button, int dir, int count)
        {
            if (dir == up)
            {
                _root_widget.mouse_click({ x, y, }, button, count);
            }

            _root_widget.mouse_button({ x, y }, button, dir == down ? cppgui::pressed : cppgui::released);
        }
        void mouse_wheel(int x, int y)
        {
            _root_widget.mouse_wheel({x, y});
        }
        void text_input(const char32_t *text, size_t size)
        {
            _root_widget.text_input(text, size);
        }
        void key_down(const Keycode &key)
        {
            _root_widget.key_down(key);
        }

        void closing()
        {
            p()->cleanup_window();
        }

    private:
        class Impl_t: public Impl { friend class GUI_window; };
        auto p() { return static_cast<Impl_t*>(this); }

        Canvas_t                   *_canvas;        // TODO: use unique_ptr<>
        Root_widget                 _root_widget;
        //bool                        _init_done = false;
    };

} // ns cppgui
