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

namespace cppgui {

    extern int dummy;

    template <class Config, bool With_layout, class Parent> struct Root_widget__Layouter;

    // Root widget

    // TODO: confer the ability to render a background ?

    template <class Config, bool With_layout>
    class Root_widget: 
        public Root_widget__Layouter<Config, With_layout, typename Config::template Root_widget__Updater< Abstract_widget<Config, With_layout> > >,
        public Config::template Root_widget__Container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        //using Renderer = typename GUIConfig::Renderer;
        using Keycode  = typename Config::Keyboard::Keycode;
        using Widget_t = typename Widget<Config, With_layout>;
        using Abstract_widget_t = typename Abstract_widget<Config, With_layout>;
        using Canvas_t = typename Abstract_widget_t::Canvas_t;
        using Abstract_container_t = Abstract_container<Config, With_layout>;
        //using Font_mapper = typename Config::Font_mapper;
        using Font_handle = typename Canvas_t::Font_handle;
        using Cursor_handle = typename Config::Mouse::Cursor_handle;

        // Root_widget(Canvas_t *);

        using Abstract_container_t::add_child;
        using Abstract_container_t::remove_child;

        void set_background_color(const Color &color) { _bkgnd_clr = color; }

        void set_canvas(Canvas_t *canvas)
        {
            _canvas = canvas;
        }
        auto canvas() const { return _canvas; }

        void init() override
        {
            init_child_resources();

            Abstract_widget_t::init();
        }
        void cleanup() // TODO: override!
        {        
            //TODO: cleanup_children_resources();

            //_canvas->cleanup();
            // see init()
        }

        void compute_view_from_data() override
        {
            compute_child_views();

            Abstract_widget_t::compute_view_from_data();
        }

        // TODO: request mechanism ?
        bool container_has_focus() override { return true; } // TODO: only return true if owning window is active ?

        //void set_focus_to(Widget_t *);
        //auto focused_widget() const { return focused_child(); } // TODO: remove and replace all calls?

        void push_cursor(Cursor_handle cursor)
        {
            _cursor_stack.push( Config::Mouse::get_current_cursor() );

            Config::Mouse::set_cursor(cursor);
        }
        void pop_cursor()
        {
            // All cursors but the top one must be freed after use
            if (_cursor_stack.size() > 1)
            {
                Config::Mouse::free_cursor( Config::Mouse::get_current_cursor() );
            }

            Config::Mouse::set_cursor(_cursor_stack.top());

            _cursor_stack.pop();
        }

        void mouse_motion(const Point &pos) override
        {
            this->lock();

            if (_mouse_holder)
            {
                _mouse_holder->mouse_motion(pos - _capture_offset);
            }

            // Note: there should be no harm in repeating a mouse motion event (contrary to e.g. mouse button events)
            //  (we need to repeat the event so as to support drag-and-drop, e.g. highlighting potential targets)
            // TODO: this will trigger mouse_exit() even if the mouse was captured - is that correct/acceptable ?
            container_mouse_motion(pos);

            this->unlock();
        }
        void mouse_button(const Point &pos, int button, Key_state state) override
        {
            this->lock();

            if (_mouse_holder)
            {
                _mouse_holder->mouse_button(pos, button, state);
            }
            else
            {
                container_mouse_button(pos, button, state); // Abstract_container TODO: better name ?
            }

            this->unlock();
        }
        void mouse_click(const Point &pos, int button, int count) override
        {
            this->lock();
            container_mouse_click(pos, button, count);
            this->unlock();
        }
        void mouse_wheel(const Vector &dir) override
        {
            this->lock();
            container_mouse_wheel(dir);
            this->unlock();
        }
        void text_input(const char32_t *text, size_t size) override
        {
            if (focused_child())
            {
                this->lock();
                focused_child()->text_input(text, size);
                this->unlock();
            }
        }
        void key_down(const Keycode &key)
        {
            if (focused_child())
            {
                this->lock();
                focused_child()->key_down(key);
                this->unlock();
            }
        }

        //void key_up(const Keycode &) override;

        void render()
        {
            render(_canvas, {0, 0});
        }

        void child_key_down(const Keycode &) override {}

        void capture_mouse(Widget_t *holder)
        {
            assert(!_mouse_holder);

            _mouse_holder = holder;

            // The mouse holder expects mouse positions as relative to its own origin,
            // so we get its absolute position here and store it as the "capture offset"
            Point offset = holder->position();
            for (auto cont = holder->container(); cont != this; cont = static_cast<Container_t*>(cont)->container())
            {
                offset += static_cast<Container_t*>(cont)->position();
            }
            _capture_offset = offset;
        }
        void release_mouse()
        {
            if (_mouse_holder)
            {
                assert(_mouse_holder);
                _mouse_holder = nullptr;
            }
        }
        auto mouse_holder() const { return _mouse_holder; }

    protected:
        
        void render(Canvas_t *canvas, const Point &offset) override // TODO: rename to _render() ?
        {
            auto pos = offset + position();

            if (_bkgnd_clr[3] > 0)
            {
                canvas->clear(canvas->rgba_to_native(_bkgnd_clr));
            }

            for (auto& child : children())
            {
                if (child->visible()) child->render(canvas, pos);
            }
        }

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

        void invalidate()
        {
            _on_invalidated();
        }

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
        using Container_t = Container<Config, With_layout>;
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

        auto p() { return static_cast<Root_widget_t*>(static_cast<Root_widget<Config, true>*>(this)); }

        virtual void init_layout()
        {
            p()->init_children_layout();
        }        
        virtual auto get_minimal_size() -> Extents { return {0, 0}; }
        virtual void layout()
        {
            p()->layout_children(); 
        }

        void insert_child(Widget_t *child)
        {
            p()->add_child(child);

            child->init_layout();
            child->layout();
            child->init();

            p()->invalidate();
        }
        void drop_child(Widget_t *child)
        {
            if (p()->contains_widget( p()->mouse_holder() ))
            {
                p()->release_mouse();
            }

            p()->remove_child(child); 

            p()->invalidate();
        }
    };

} // ns cppgui
