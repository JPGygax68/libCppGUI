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
#include "./Container_base.hpp"

namespace cppgui {

    extern int dummy;

    template <class Config, bool With_layout, class Parent> struct Root_widget__Layouter;

    // Root widget

    // TODO: confer the ability to render a background ?

    template <class Config, bool With_layout>
    class Root_widget: 
        public Root_widget__Layouter<Config, With_layout, 
            typename Config::template Root_widget__Container_updater<
                typename Config::template Root_widget__Updater< 
                    Container_base<Config, With_layout> > > >
    {
    public:
        //using Renderer = typename GUIConfig::Renderer;
        using Keycode  = typename Config::Keyboard::Keycode;
        using Widget_t = typename Widget<Config, With_layout>;
        //using Abstract_widget_t = typename Abstract_widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        //using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Container_base_t = Container_base<Config, With_layout>;
        //using Font_mapper = typename Config::Font_mapper;
        using Font_handle = typename Canvas_t::Font_handle;
        using Cursor_handle = typename Config::Mouse::Cursor_handle;

        // Root_widget(Canvas_t *);

        using Container_base_t::add_child;
        using Container_base_t::remove_child;

        void set_background_color(const Color &color) { _bkgnd_clr = color; }

        void set_canvas(Canvas_t *);
        auto canvas() const { return _canvas; }

        void init() override;
        void cleanup(); // TODO: override!

        void compute_view_from_data() override;

        // TODO: request mechanism ?
        bool has_focus() override { return true; } // TODO: return window activation state ?
        bool container_has_focus() override { return true; } // TODO: only return true if owning window is active ?

        //void set_focus_to(Widget_t *);
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

        void render();

        void child_key_down(const Keycode &) override {}

        void capture_mouse(Widget_t *);
        void release_mouse();
        auto mouse_holder() const { return _mouse_holder; }

    protected:
        
        void render(Canvas_t *, const Point &) override;

    private:
        Color                       _bkgnd_clr = { 0, 0, 0, 0 };
        Canvas_t                   *_canvas = nullptr;
        //Widget_t                   *_focused_widget = nullptr;
        std::stack<Cursor_handle>   _cursor_stack;
        Widget_t                   *_mouse_holder = nullptr;
        Point                       _capture_offset;
    };

    // Default implementation for Widget_updater aspect

    template <class Config, bool With_layout, class Parent>
    struct Default__Root_widget__Updater: public Parent 
    {
        using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Invalidated_handler = std::function<void()>;
        using Root_widget_t = Root_widget<Config, With_layout>;

        auto root_widget() { return p(); }

        void invalidate();

        void on_invalidated(Invalidated_handler handler) { _on_invalidated = handler; }

    private:
        auto p() -> Root_widget_t * { return static_cast<Root_widget_t*>(static_cast<Root_widget<Config, true>*>(this)); }

        Invalidated_handler _on_invalidated;
    };

    // Default implementation for Container_updater aspect

    template <class Config, bool With_layout, class Parent>
    struct Default__Root_widget__Container_updater: public Parent
    {
        using Widget_t = Widget<Config, With_layout>;
        using Container_base_t = Container_base<Config, With_layout>;
        using Root_widget_t = Root_widget<Config, With_layout>;

        // Container_updater contract

        void child_invalidated(Widget_t *) override { _must_update = true; }

        auto container_root_widget() -> Root_widget_t * override { return static_cast<Root_widget_t*>(this); }

        // Specific functionality 

        void lock() { _must_update = false; }

        void unlock() { if (_must_update) p()->invalidate(); }

    private:
        auto p() { return static_cast<Root_widget_t*>(static_cast<Root_widget<Config, With_layout>*>(this)); }

        bool                _must_update;
    };

    // Layouting aspect

    template <class Config, class Parent> 
    struct Root_widget__Layouter<Config, true, Parent>: public Parent 
    {
        class Root_widget_t: public Root_widget<Config, true> { friend struct Root_widget__Layouter; };

        using Widget_t = Widget<Config, true>;

        auto p() { return static_cast<Root_widget_t*>(this); }

        virtual void init_layout();
        virtual auto get_minimal_size() -> Extents { return {0, 0}; }
        virtual void layout();

        void insert_child(Widget_t *);
        void drop_child(Widget_t *);
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_ROOT_WIDGET(Config, With_layout) \
    template cppgui::Root_widget<Config, With_layout>; \
    template cppgui::Root_widget__Layouter<Config, With_layout, \
        Config::template Root_widget__Container_updater< \
            Config::template Root_widget__Updater< \
                cppgui::Container_base<Config, With_layout> > > >;

