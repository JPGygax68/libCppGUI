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

#include <algorithm>
#include <Container.hpp>


namespace cppgui {

    void Container_base::set_initial_focus(Widget *child)
    {
        _focused_child = child;
    }

    auto Container_base::container_absolute_position() -> Point
    {
        return container()->container_absolute_position() + position();
    }

    void Container_base::set_focus_on_child(Widget *child)
    {
        if (child != _focused_child)
        {
            if (_focused_child)
            {
                _focused_child->loosing_focus(); // TODO: support veto-ing loss of focus ?
            }

            _focused_child = child;

            if (_focused_child)
            {
                _focused_child->gained_focus();
                pass_up_and_notify_focus();
            }
        }
    }

    auto Container_base::child_index(Widget *child) -> Index
    {
        using namespace std;

        return distance(begin(_children), find(begin(_children), end(_children), child) );
    }

    auto Container_base::child_index(Widget *c) const -> Index
    {
        using namespace std;

        return find(begin(children()), end(children()), c) - begin(children());
    }

    void Container_base::add_child(Widget *child)
    {
        _children.push_back(child);
        child->added_to_container(this);
    }

    void Container_base::remove_child(Widget *child)
    {
        if (child == _hovered_child)
        {
            _hovered_child->mouse_exit();
            _hovered_child = nullptr;
        }

        if (child == _focused_child)
        {
            _focused_child = nullptr;
        }

        child->removed_from_container(this);

        auto it = find(begin(_children), end(_children), child);
        assert(it != std::end(_children));
        _children.erase(it);
    }

    void Container_base::remove_all_children()
    {
        for (auto child: _children)
        {
            remove_child(child);
        }
    }

    void Container_base::compute_view_from_data()
    {
        compute_child_views();
    }

    void Container_base::init(Canvas *c)
    {
        init_child_resources(c);
    }

    void Container_base::render(Canvas *c, const Point &offs)
    {
        auto p = offs + position();
        // c->fill_rect(Rectangle{bounds()} + p, background_color(visual_states()));

        // This base implementation just renders the children, without border or background

        render_children(c, p);
    }

    bool Container_base::mouse_motion(const Point &p)
    {
        auto hovered = child_at(p);

        if (hovered != _hovered_child)
        {
            /** TODO: this does not account for situations where the pointer enters a
                widget, then moves on to a zone on a higher Z-level (Z levels not being 
                implemented yet at the time of writing).
            */
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }

        if (_hovered_child)
        {
            return _hovered_child->mouse_motion(p - _hovered_child->position());
        }

        return false;
    }

    bool Container_base::mouse_button(const Point &p, int button, Key_state state, Count clicks)
    {
        auto child = child_at(p);

        if (child) return child->mouse_button(p - child->position(), button, state, clicks);

        return false;
    }

    void Container_base::mouse_exit()
    {
        // If the mouse leaves the container, it also leaves the children

        if (_hovered_child)
        {
            _hovered_child->mouse_exit();
            _hovered_child = nullptr;
        }

        Super::mouse_exit();
    }

    bool Container_base::cycle_focus(int steps)
    {
        if (steps > 0)
        {
            while (steps-- > 0) return cycle_focus_forward();
        }
        else if (steps < 0)
        {
            while (steps++ < 0) return cycle_focus_backward();
        }

        return false;
    }

    bool Container_base::cycle_focus_forward()
    {
        assert(has_focus());

        if (children().empty())
        {
            return false; // cannot cycle, report back to sender
        }
        
        std::vector<Widget*>::const_iterator it;

        auto target = focused_child();
        if (target)
        {
            it = std::find(std::begin(children()), std::end(children()), target);  
            ++it;
        }
        else {
            it = std::begin(children());
        }

        while (it != std::end(children()) && !(*it)->focussable()) ++it;

        if (it != std::end(children()))
        {
            //(*it)->gained_focus();
            set_focus_on_child(*it);
            return true;
        }

        set_focus_on_child(nullptr);
        return false; // we could not (quite) cycle, caller try again
    }

    bool Container_base::cycle_focus_backward()
    {
        assert(has_focus());

        if (children().empty())
        {
            return false; // cannot cycle, report back to sender
        }

        //decltype(std::rbegin(children())) it;
        Child_list::const_reverse_iterator it;

        // Does one of the children have focus ?
        if (focused_child())
        {
            // Yes: move to the predecessor
            auto target = focused_child();
            it = std::find(std::rbegin(children()), std::rend(children()), target); // focused_child());  
            ++it;
        }
        else {
            // No: select the last child
            it = std::rbegin(children());
        }

        // Skip non-focussable children
        while (it != std::rend(children()) && !(*it)->focussable()) ++it;

        // Ended up on a child ?
        if (it != std::rend(children()))
        {
            // Inform child it gained focus
            //(*it)->gained_focus();
            set_focus_on_child(*it);
            return true;
        }

        set_focus_on_child(nullptr);
        return false; // we could not (quite) cycle, caller try again
    }

    auto Container_base::child_at(const Point &pos) -> Widget*
    {
        auto child = std::find_if(begin(_children), end(_children), [&](auto ch) { 
            return ch->visible() && ch->contains_point(pos); 
        });

        return child != end(_children) ? *child : nullptr;
    }

    void Container_base::init_child_resources(Canvas *c)
    {
        for (auto child : children())
        {
            child->init(c);
        }
    }

    void Container_base::compute_child_views()
    {
        for (auto& child : children())
        {
            child->compute_view_from_data();
        }
    }

    /*
     * By convention, the offs (offset) parameter specified here must already contain
     * the position of the container itself.
     */
    void Container_base::render_children(Canvas *c, const Point &offs)
    {
        for (auto& child : children())
        {
            if (child->visible()) 
            {
                child->render(c, offs);
            }
        }
    }

    /*
    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_click(const Point &pos, int button, Count count)
    {
        auto child = child_at(pos);

        if (child) child->mouse_click(pos - child->position(), button, count);
    }
    */

    bool Container_base::mouse_wheel(const Vector &v)
    {
        if (_hovered_child) return _hovered_child->mouse_wheel(v);

        return false;
    }

    bool Container_base::text_input(const char32_t *text, size_t size)
    {
        if (_focused_child)
        {
            return _focused_child->text_input(text, size);
        }

        return Super::text_input(text, size);
    }

    bool Container_base::key_down(const Keycode &key)
    {
        if (_focused_child)
        {
            if (_focused_child->key_down(key)) return true;
        }

        if (is_tab(key) && !is_shift_down()) return cycle_focus( 1);
        if (is_tab(key) &&  is_shift_down()) return cycle_focus(-1);

        return false;
    }

    template<class Pred>
    auto Container_base::scan_children_forward(Index from, Pred pred) -> Index
    {
        for (auto i = from; i < (Index) _children.size(); i ++)
        {
            if (pred(_children[i])) return i;
        }

        return (Index) _children.size();
    }

    template<class Pred>
    auto Container_base::scan_children_backward(Index from, Pred pred) -> Index
    {
        for (Index i = from; i >= 0; i--)
        {
            if (pred(_children[i])) return i;
        }

        return - 1;
    }

    bool Container_base::contains_widget(Widget *widget)
        // Recursively check whether this or a descendant widget contains the specified widget.
    {
        for (auto child: _children)
        {
            if (child == widget)
            {
                return true;
            }

            auto cont = dynamic_cast<decltype(this)>(child);
            if (cont && cont->contains_widget( widget )) 
            {
                return true;
            }
        }

        return false;
    }

    // "Updater" aspect

    void Container_base::child_invalidated(Widget *)
    {
        container()->child_invalidated(this);
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Container_base::init_layout()
    {
        init_children_layout();
    }

    void Container_base::init_children_layout()
    {
        for (auto child : children())
        {
            child->init_layout();
        }
    }

#endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui
