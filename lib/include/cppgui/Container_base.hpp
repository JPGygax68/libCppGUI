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

#include "./Container.hpp"
//#include "./Box_model.hpp"


namespace cppgui {

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget.
     */

    class Container_base: public Widget, public Abstract_container
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

        auto container_absolute_position() -> Point override;
        bool container_has_focus() override { return has_focus(); }

        void switch_focused_child(Widget *child) override;
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

        void render(Canvas *, const Point &) override;

        // Updater aspect

        void child_invalidated(Widget *) override;

        auto container_root_widget() { return root_widget(); }

    protected:

        bool handle_key_down(const Keycode &) override;

        // Actions (return value indicates success)
        bool cycle_focus_forward ();
        bool cycle_focus_backward();
    };

} // ns cppgui
