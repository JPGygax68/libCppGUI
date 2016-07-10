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

#include <memory>

#include "./Widget.hpp"
#include "./layout_managers.hpp"

namespace cppgui {

    template <class Config, bool With_layout, class Parent> struct Abstract_container__Layouter;

    /** Container functionality (ability to contain Widgets).
    */

    #define CPPGUI_INSTANTIATE_ABSTRACT_CONTAINER(Config, With_layout) \
        template cppgui::Abstract_container<Config, With_layout>; \
        template cppgui::Abstract_container__Layouter<Config, With_layout, cppgui::Nil_struct>;

    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_Container_updater<Nil_struct>,
        public Abstract_container__Layouter<Config, With_layout, Nil_struct>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Keyboard = typename Config::Keyboard;
        using Keycode  = typename Keyboard::Keycode;

        Abstract_container(): _hovered_child { nullptr } {}

        void set_initial_focus(Widget_t *);

        auto& children() { return _children; }

        /** Called when a key_down event could not be handled by the child it was sent to
            (from container_key_down()) and needs to "bubble" back up.
         */
        virtual void child_key_down(const Keycode &) = 0;

        virtual bool container_has_focus() = 0;       
        virtual auto container_absolute_position() -> Point = 0;
        virtual void switch_focused_child(Widget_t *);
        auto focused_child() -> Widget_t * { return _focused_child; }

        auto first_child() -> Widget_t * { assert(!_children.empty()); return _children.front(); }
        auto last_child () -> Widget_t * { assert(!_children.empty()); return _children.back (); }

        auto child_index(Widget_t *child) -> Index;

        template<class Pred> auto scan_children_forward (Index from, Pred) -> Index;
        template<class Pred> auto scan_children_backward(Index from, Pred) -> Index;

    protected:

        void add_child(Widget_t *);
        // TODO: should removal methods be moved to optional aspect ?
        void remove_child(Widget_t *);
        void remove_all_children();

        auto child_at(const Point &) -> Widget_t *;

        void init_child_resources();
        void compute_child_views();

        void render_children(Canvas_t *, const Point &offs);

        /** The container_xxxx() methods are intended as "delegate" event handlers, to be 
            called from "real" containers (i.e. descendants of Container<>).            
        */
        void container_mouse_motion(const Point &);
        void container_mouse_button(const Point &, int button, Key_state, Count clicks);
        //void container_mouse_click(const Point &, int button, int count);
        void container_mouse_wheel(const Point &dist);
        void container_mouse_exit();
        void container_text_input(const char32_t *, size_t);
        bool container_key_down(const Keycode &);

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
        Widget_t *_focused_child = nullptr;
    };

    template <class Impl, class Config, bool With_layout> class Container_base;

    // Container_updater aspect -------------------------------------

    // IMPORTANT! This is *different* from the "Updater" aspect, which belongs to Widgets!

    template<class Config, bool With_layout>
    struct Default_Abstract_container_Container_updater
    {
        template<class Impl, class Parent>
        struct Aspect
        {
            using Widget_t = Widget<Config, With_layout>;
            using Root_widget_t = Root_widget_base<Config, With_layout>;

            virtual ~Aspect() {}

            virtual void child_invalidated(Widget_t *) = 0;

            virtual auto container_root_widget() -> Root_widget_t * = 0;

        protected:
            class Container_t: public Impl { friend struct Aspect; };
            auto p() { return static_cast<Container_t*>(this); }
        };
    };

    // Layouter aspect ----------------------------------------------

    // Dummy implementation

    template <class Config, class Parent> 
    struct Abstract_container__Layouter<Config, false, Parent>: Parent { };

    // Real implementation

    template <class Config, class Parent> 
    struct Abstract_container__Layouter<Config, true, Parent>: Parent
    {
        using Widget_t = typename Widget<Config, true>;
        class Abstract_container_t: public Abstract_container<Config, true> { friend struct Abstract_container__Layouter; };

        void init_children_layout();

    protected:
        bool contains_widget(Widget_t *);

    private:
        auto p() { return static_cast<Abstract_container_t*>(static_cast<Abstract_container<Config, true>*>(this)); }
    };

} // ns cppgui
