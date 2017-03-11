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

#include <Container.hpp>

namespace cppgui {

    void Container::set_initial_focus(Widget *child)
    {
        _focused_child = child;
    }

    auto Container::container_absolute_position() -> Point
    {
        return container()->container_absolute_position() + position();
    }

    void Container::switch_focused_child(Widget *child)
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
            }
        }
    }

    auto Container::child_index(Widget *child) -> Index
    {
        using namespace std;

        return distance(begin(_children), find(begin(_children), end(_children), child) );
    }

    void Container::add_child(Widget *child)
    {
        _children.push_back(child);
        child->added_to_container(this);
    }

    void Container::remove_child(Widget *child)
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

    void Container::remove_all_children()
    {
        for (auto child: _children)
        {
            remove_child(child);
        }
    }

    void Container::init(Canvas *c)
    {
        init_child_resources(c);
    }

    void Container::mouse_motion(const Point &p)
    {
        container_mouse_motion(p);
    }

    void Container::child_key_down(const Keycode &key)
    {
        if (!handle_key_down(key))
        {
            this->container()->child_key_down(key);
        }
    }

    auto Container::child_at(const Point &pos) -> Widget*
    {
        auto child = std::find_if(begin(_children), end(_children), [&](auto ch) { 
            return ch->visible() && ch->contains_point(pos); 
        });

        return child != end(_children) ? *child : nullptr;
    }

    void Container::init_child_resources(Canvas *c)
    {
        for (auto child : children())
        {
            child->init(c);
        }
    }

    void Container::compute_child_views()
    {
        for (auto& child : children())
        {
            child->compute_view_from_data();
        }
    }

    void Container::render_children(Canvas *cv, const Point & offs)
    {
        for (auto& child : children())
        {
            if (child->visible()) 
            {
                child->render(cv, offs);
            }
        }
    }

    void Container::container_mouse_motion(const Point &pos)
    {
        auto hovered = child_at(pos);

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
            _hovered_child->mouse_motion(pos - _hovered_child->position());
        }
    }

    void Container::container_mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        auto child = child_at(pos);

        if (child) child->mouse_button(pos - child->position(), button, state, clicks);
    }

    /*
    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_click(const Point &pos, int button, Count count)
    {
        auto child = child_at(pos);

        if (child) child->mouse_click(pos - child->position(), button, count);
    }
    */

    void Container::container_mouse_wheel(const Vector & dist)
    {
        if (_hovered_child) _hovered_child->mouse_wheel(dist);
    }

    void Container::container_mouse_exit()
    {
        // We must propagate the exit to any currently hovered child
        if (_hovered_child)
        {
            _hovered_child->mouse_exit();
            _hovered_child = nullptr;
        }
    }

    void Container::container_text_input(const char32_t *text, size_t size)
    {
        if (_focused_child)
        {
            _focused_child->text_input(text, size);
        }
    }

    bool Container::container_key_down(const Keycode &key)
    {
        if (_focused_child)
        {
            return _focused_child->handle_key_down(key);
        }
        else
            return true;
    }

    template<class Pred>
    auto Container::scan_children_forward(Index from, Pred pred) -> Index
    {
        for (auto i = from; i < (Index) _children.size(); i ++)
        {
            if (pred(_children[i])) return i;
        }

        return (Index) _children.size();
    }

    template<class Pred>
    auto Container::scan_children_backward(Index from, Pred pred) -> Index
    {
        for (Index i = from; i >= 0; i--)
        {
            if (pred(_children[i])) return i;
        }

        return - 1;
    }

    bool Container::contains_widget(Widget *widget)
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

    void Container::child_invalidated(Widget *)
    {
        container()->child_invalidated(this);
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Container::init_layout()
    {
        init_children_layout();
    }

    void Container::init_children_layout()
    {
        for (auto child : children())
        {
            child->init_layout();
        }
    }

#endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui
