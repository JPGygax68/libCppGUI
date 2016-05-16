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

#include <algorithm>

#include "./Container_base.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::init()
    {
        init_child_resources();

        Widget_t::init();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::container_take_focus(Widget_t *child)
    {
        // Inform former focused child, update focused_child property
        Abstract_container_t::container_take_focus(child);

        // Propagate
        //container()->container_take_focus(this);
        pass_up_and_notify_focus();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::gained_focus()
    {
        if (_focused_child)
        {
            _focused_child->gained_focus();
        }

        Widget_t::gained_focus();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::loosing_focus()
    {
        Widget_t::loosing_focus();

        if (_focused_child)
        {
            _focused_child->loosing_focus();
        }
    }

    template <class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_motion(const Point &pos)
    {
        container_mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state)
    {
        container_mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_click(const Point &pos, int button, int count)
    {
        Widget_t::mouse_click(pos, button, count);

        container_mouse_click(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_wheel(const Vector &dist)
    {
        container_mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::text_input(const char32_t *text, size_t size)
    {
        container_text_input(text, size);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::key_down(const Keycode &key)
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

    template<class Config, bool With_layout>
    bool Container_base<Config, With_layout>::handle_key_down(const Keycode &key)
    {
        if (Config::Keyboard::is_tab(key))
        {
            if (!Config::Keyboard::is_shift_down()) return cycle_focus_forward ();  // failure will make the event bubble up
            else                                    return cycle_focus_backward();  // "
        }

        // Delegate to container key down handler
        return container_key_down(key);
    }

    template<class Config, bool With_layout>
    bool Container_base<Config, With_layout>::cycle_focus_forward()
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

    template<class Config, bool With_layout>
    bool Container_base<Config, With_layout>::cycle_focus_backward()
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

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_exit()
    {
        container_mouse_exit();
        Widget_t::mouse_exit();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::child_key_down(const Keycode & key)
    {
        if (!handle_key_down(key))
        {
            container()->child_key_down(key);
        }
    }

    template <class Config, bool With_layout>
    void Container_base<Config, With_layout>::render(Canvas_t *cv, const Point &offs)
    {
        fill(cv, offs, background_color());

        render_children(cv, offs + position());
    }

} // ns cppgui