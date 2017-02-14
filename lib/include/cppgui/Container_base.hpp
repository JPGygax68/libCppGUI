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

#include "./Widget.hpp"
#include "./Abstract_container.hpp"
#include "./Box_model.hpp"

namespace cppgui {

    // Forward declaration

    template <class Config, bool With_layout> class Container_base;

    // Internal (templated) pseudo-namespace 

    template<class Config>
    struct _container_base
    {
        template <bool With_layout, class Parent> struct Layouter;

        // Base class -----------------------------------------------

        template <bool With_layout>
        class _Base: public Widget<Config, With_layout>, public Abstract_container<Config, With_layout>
        {            
        };

        // Layouter -------------------------------------------------

        // Dummy implementation

        template<class Parent>
        struct Layouter<false, Parent>: Parent
        {            
        };

        // Real implementation

        template<class Parent>
        struct Layouter<true, Parent>: Parent
        {
            using Widget_t = typename Widget<Config, true>;

            // Contract

            void init_layout() override;
            //auto get_minimal_size() -> Extents override;
            void layout() override;

            // New methods

            void insert_child(Widget_t *); // TODO: find a better name OR support insertion index
            void drop_child  (Widget_t *);

        private:
            class Container_base_t: public Container_base<Config, true> { friend struct Layouter; };
            auto p() { return static_cast<Container_base_t*>(this); }

        };
    };

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget.
     */

    template <class Config, bool With_layout>
    class Container_base: public
        Config::template Container_base__Container_updater<
            _container_base<Config>::template Layouter<With_layout,
                _container_base<Config>::template _Base<With_layout> > >
    {
    public:
        #ifdef OBSOLETE
        using Renderer = typename Config::Renderer;
        using Widget_t = Widget<Config, With_layout>;
        using Parent_t = Widget_t;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;
        #endif

        void init() override;

        void compute_view_from_data() override;

        using Abstract_container_t::add_child;
        using Abstract_container_t::remove_child;
        using Abstract_container_t::remove_all_children;

        auto container_absolute_position() -> Point override;
        bool container_has_focus() override { return this->has_focus(); }

        void switch_focused_child(Widget_t *child) override;
        void gained_focus() override;
        void loosing_focus() override;

        void mouse_motion(const Point &) override;
        void mouse_button(const Point &, int button, Key_state, Count clicks) override;
        //void mouse_click(const Point &, int button, int count) override;
        void mouse_wheel(const Vector &) override;
        void text_input(const char32_t *, size_t) override;
        void key_down(const Keycode &) override;

        void mouse_exit() override;

        /** Handle key_down event that has "bubbled" back up from the child it was send to.
         */
        void child_key_down(const Keycode &key) override;

        void render(Canvas_t *, const Point &) override;

    protected:

        bool handle_key_down(const Keycode &) override;

        // Actions (return value indicates success)
        bool cycle_focus_forward ();
        bool cycle_focus_backward();
    };

    // Container_base_updater aspect

    template <class Config, bool With_layout, class Parent>
    struct Default__Container_base__Container_updater: Parent 
    {
        class Container_base_t: public Container_base<Config, true> { friend struct Default__Container_base__Container_updater; };
        using Widget_t = Widget<Config, true>;

        auto p() { return static_cast<Container_base_t*>(static_cast<Container_base<Config, true>*>(this)); }

        void child_invalidated(Widget_t *) override;

        auto container_root_widget() { return p()->root_widget(); }
    };

} // ns cppgui
