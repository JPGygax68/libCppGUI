#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016, 2017 Hans-Peter Gygax

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


namespace cppgui {

    class Root_widget: public Container_base
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

        //void set_focus_to(Widget *);
        //auto focused_widget() const { return focused_child(); } // TODO: remove and replace all calls?

        void push_cursor(Cursor_handle);
        void pop_cursor();

        // TODO: replace the following event override with standalone methods that inject to container_xxxx()
        bool mouse_motion(const Point &) override;
        bool mouse_button(const Point &, int button, Key_state, Count clicks) override;
        //bool mouse_click(const Point &, int button, int count);
        bool mouse_wheel(const Vector &) override;
        bool text_input(const char32_t *, size_t) override;
        bool key_down(const Keycode &) override;
        //bool key_up(const Keycode &);

        void render(Canvas *c, const Point &p) override;

        void capture_mouse(Widget *);
        void release_mouse();
        auto mouse_holder() const { return _mouse_holder; }

        // Specific functionality

        void insert_child(Widget * child, const Point &p, Bbox_cref b, Canvas *c);
        void drop_child(Widget * child);

    private:
        using Super = Container_base;

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
        auto get_minimal_bounds() -> Bbox override;
        void compute_layout(Bbox_cref b) override;

    #endif // CPPGUI_EXCLUDE_LAYOUTING

    }; // class Root_widget

} // ns cppgui
