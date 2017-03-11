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

#include <functional>
#include <stack>

#include "./Widget.hpp"
#include "./Container.hpp"
//#include "./Container_layouter.hpp"
//#include "./Box_model.hpp"


#pragma warning(disable: 4505)

namespace cppgui {

    class Root_widget: public Container
    {
    public:

        Root_widget()
        {
            this->set_id("Root_widget_base");
        }

        void set_background_color(const RGBA &color) { _bkgnd_clr = color; }

        //void set_canvas(Canvas *);
        //auto canvas() const { return _canvas; }

        void init(Canvas *) override;
        void cleanup(); // TODO: override!

        void compute_view_from_data() override;

        // TODO: request mechanism ?
        bool has_focus() override { return true; } // TODO: return window activation state ?
        auto container_absolute_position() -> Point override;
        bool container_has_focus() override { return true; } // TODO: only return true if owning window is active ?

        //void set_focus_to(Widget *);
        //auto focused_widget() const { return focused_child(); } // TODO: remove and replace all calls?

        void push_cursor(Cursor_handle);
        void pop_cursor();

        // TODO: replace the following event override with standalone methods that inject to container_xxxx()
        void mouse_motion(const Point &);
        void mouse_button(const Point &, int button, Key_state, Count clicks);
        //void mouse_click(const Point &, int button, int count);
        void mouse_wheel(const Vector &);
        void text_input(const char32_t *, size_t);
        void key_down(const Keycode &);
        //void key_up(const Keycode &);

        void render(Canvas *c, const Point &p) override;

        void child_key_down(const Keycode &) override;

        void capture_mouse(Widget *);
        void release_mouse();
        auto mouse_holder() const { return _mouse_holder; }

        // Specific functionality

        void insert_child(Widget * child, const Point &p, const Bounding_box &b, Canvas *c);
        void drop_child(Widget * child);

    private:
        RGBA                        _bkgnd_clr = { 0, 0, 0, 0 };
        //Canvas                     *_canvas = nullptr;
        std::stack<Cursor_handle>   _cursor_stack;
        Widget                     *_mouse_holder = nullptr;
        Point                       _capture_offset;

    // Updating aspect ------------------------------------
    
    public:
        using Invalidated_handler = std::function<void()>;

        void invalidate();
        void on_invalidated(Invalidated_handler handler) { _on_invalidated = handler; }

    private:
        Invalidated_handler _on_invalidated;


        // Container_updater aspect ---------------------------------

    public:

        // Contract
        void child_invalidated(Widget *) override { _must_update = true; }
        auto container_root_widget() -> Root_widget * override { return static_cast<Root_widget*>(this); }

        // Specific functionality 
        void lock() { _must_update = false; }
        void unlock() { if (_must_update) invalidate(); }
    
    private:

        bool                _must_update;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:

        void init_layout() override;
        auto get_minimal_bounds() -> Bounding_box override;
        void set_bounds(const Point & p, const Bounding_box & b) override;

    #endif // CPPGUI_EXCLUDE_LAYOUTING

    }; // class Root_widget

} // ns cppgui
