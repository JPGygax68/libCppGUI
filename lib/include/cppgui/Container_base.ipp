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

#include <algorithm>

#include "./Container_base.hpp"

#include "./Abstract_container.ipp"

namespace cppgui {

    // Main class ---------------------------------------------------

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::init()
    {
        this->init_child_resources();

        this->Widget_t::init();
    }

    template <class Config, bool With_layout>
    void Container_base<Config, With_layout>::compute_view_from_data()
    {
        this->compute_child_views();

        this->Widget_t::compute_view_from_data();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::container_take_focus(Widget_t *child)
    {
        // Inform former focused child, update focused_child property
        Abstract_container_t::container_take_focus(child);

        // Propagate
        //container()->container_take_focus(this);
        this->pass_up_and_notify_focus();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::gained_focus()
    {
        if (this->_focused_child)
        {
            this->_focused_child->gained_focus();
        }

        Widget_t::gained_focus();
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::loosing_focus()
    {
        Widget_t::loosing_focus();

        if (this->_focused_child)
        {
            this->_focused_child->loosing_focus();
        }
    }

    template <class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_motion(const Point &pos)
    {
        this->container_mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        // TODO: call Widget_t::mouse_button() if container_mouse_button() did nothing
        this->container_mouse_button(pos, button, state, clicks);
    }

    #ifdef NOT_DEFINED

    template<class Config, bool With_layout>
    void Container_base<Config, With_layout>::mouse_click(const Point &pos, int button, Count count)
    {
        container_mouse_click(pos, button, count);
    }

    #endif

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
        assert(this->has_focus());

        if (this->children().empty())
        {
            return false; // cannot cycle, report back to sender
        }
        else
        {
            typename std::vector<Widget_t*>::iterator it;

            if (this->focused_child())
            {
                it = std::find(std::begin(this->children()), std::end(this->children()), this->focused_child());  
                it ++;
            }
            else {
                it = std::begin(this->children());
            }

            while (it != std::end(this->children()) && !(*it)->focussable()) it ++;

            if (it != std::end(this->children()))
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
                container_take_focus(nullptr);
                return false;
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
            this->container()->child_key_down(key);
        }
    }

    template <class Config, bool With_layout>
    void Container_base<Config, With_layout>::render(Canvas_t *cv, const Point &offs)
    {
        fill(cv, offs, this->background_color());

        render_children(cv, offs + this->position());
    }

    // Container_updater aspect -----------------------------------------------

    template<class Config, bool With_layout, class Parent>
    void Default__Container_base__Container_updater<Config, With_layout, Parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

    // Layouter aspect --------------------------------------------------------

    template <class Config>
    template <class Parent>
    void _container_base<Config>::Layouter<true, Parent>::init_layout()
    {
        p()->init_children_layout();
    }

    template <class Config>
    template <class Parent>
    auto _container_base<Config>::Layouter<true, Parent>::get_minimal_size() -> Extents
    {
        return p()->compute_minimal_size();
    }

    template <class Config>
    template <class Parent>
    void _container_base<Config>::Layouter<true, Parent>::layout()
    {
        p()->layout_children( p()->extents() );
    }

    template <class Config>
    template <class Parent>
    void _container_base<Config>::Layouter<true, Parent>::insert_child(Widget_t *child)
    {
        p()->add_child(child);

        layout();
    }

    template <class Config>
    template <class Parent>
    void _container_base<Config>::Layouter<true, Parent>::drop_child(Widget_t *child)
    {
        if (this->contains_widget( this->root_widget()->mouse_holder() ))
        {
            this->root_widget()->release_mouse();
        }

        p()->remove_child(child);

        layout();
    }

} // ns cppgui