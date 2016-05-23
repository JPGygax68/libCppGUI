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

namespace cppgui {

    // Forward declarations

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget.
     */
    template <class Config, bool With_layout>
    class Container_base: 
        public Widget<Config, With_layout>, 
        public Config::template Container_base__Container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Abstract_container_t = typename Abstract_container<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;

        void init() override
        {
            init_child_resources();

            Widget_t::init();
        }

        using Abstract_container_t::add_child;
        using Abstract_container_t::remove_child;
        using Abstract_container_t::remove_all_children;

        bool container_has_focus() override { return has_focus(); }

        void container_take_focus(Widget_t *child) override
        {
            // Inform former focused child, update focused_child property
            Abstract_container_t::container_take_focus(child);

            // Propagate
            //container()->container_take_focus(this);
            pass_up_and_notify_focus();
        }
        void gained_focus() override
        {
            if (_focused_child)
            {
                _focused_child->gained_focus();
            }

            Widget_t::gained_focus();
        }
        void loosing_focus() override
        {
            Widget_t::loosing_focus();

            if (_focused_child)
            {
                _focused_child->loosing_focus();
            }
        }

        void mouse_motion(const Point &pos) override
        { 
            container_mouse_motion(pos); 
        }
        void mouse_button(const Point &pos, int button, Key_state state) override
        {
            container_mouse_button(pos, button, state);
        }
        void mouse_click(const Point &pos, int button, int count) override
        {
            container_mouse_click(pos, button, count);
        }
        void mouse_wheel(const Vector &dist) override
        {
            container_mouse_wheel( dist );
        }
        void text_input(const char32_t *text, size_t size) override
        {
            container_text_input(text, size);
        }
        void key_down(const Keycode &key) override
        {
            // If a child has focused, pass it on to that child, otherwise fall back to
            // Widget default behaviour (i.e. try to handle locally, then bubble back up)

            if (focused_child())
            {
                focused_child()->key_down(key);
            }
            else {
                Widget_t::key_down(key); // try to handle locally, bubble up if that fails
            }
        }

        void mouse_exit() override
        {
            container_mouse_exit();
            Widget_t::mouse_exit();
        }

        /** Handle key_down event that has "bubbled" back up from the child it was send to.
         */
        void child_key_down(const Keycode &key) override
        {
            if (!handle_key_down(key))
            {
                container()->child_key_down(key);
            }
        }

        void render(Canvas_t *canvas, const Point &offs) override
        {
            fill(canvas, offs, background_color());

            render_children(canvas, offs + position());
        }

    protected:

        bool handle_key_down(const Keycode &key) override
        {
            if (Config::Keyboard::is_tab(key))
            {
                if (!Config::Keyboard::is_shift_down()) return cycle_focus_forward ();  // failure will make the event bubble up
                else                                    return cycle_focus_backward();  // "
            }

            // Delegate to container key down handler
            return container_key_down(key);
        }

        // Actions (return value indicates success)
        bool cycle_focus_forward ()
        {
            assert(has_focus());

            if (children().empty())
            {
                return false; // cannot cycle, report back to sender
            }
            else
            {
                std::vector<Widget_t*>::iterator it;

                if (focused_child())
                {
                    it = std::find(std::begin(children()), std::end(children()), focused_child());  
                    it ++;
                }
                else {
                    it = std::begin(children());
                }

                while (it != std::end(children()) && !(*it)->focussable()) it ++;

                if (it != std::end(children()))
                {
                    (*it)->gained_focus();
                    container_take_focus(*it);
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        bool cycle_focus_backward()
        {
            assert(has_focus());

            if (children().empty())
            {
                return false; // cannot cycle, report back to sender
            }
            else
            {
                decltype(std::rbegin(children())) it;

                if (focused_child())
                {
                    it = std::find(std::rbegin(children()), std::rend(children()), focused_child());  
                    it ++;
                }
                else {
                    it = std::rbegin(children());
                }

                while (it != std::rend(children()) && !(*it)->focussable()) it ++;

                if (it != std::rend(children()))
                {
                    (*it)->gained_focus();
                    container_take_focus(*it);
                    return true;
                }
                else {
                    return false; // container_take_focus(nullptr);
                }

            }
        }
    };

    // Container_base_updater aspect

    template <class Config, bool With_layout, class Parent>
    struct Default__Container_base__Container_updater: public Parent 
    {
        class Container_base_t: public Container_base<Config, true> { friend struct Default__Container_base__Container_updater; };
        using Widget_t = Widget<Config, true>;

        auto p() { return static_cast<Container_base_t*>(static_cast<Container_base<Config, true>*>(this)); }

        void child_invalidated(Widget_t *) override
        {
            p()->container()->child_invalidated(p());
        }

        auto container_root_widget() { return p()->root_widget(); }
    };

} // ns cppgui
