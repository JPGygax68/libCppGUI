#pragma once

#include <algorithm>

#include "./Abstract_container.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    inline void Abstract_container<Config, With_layout>::add_child(Widget_t *child)
    {
        _children.push_back(child);
        child->added_to_container(this);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::remove_child(Widget_t *child)
    {
        auto it = std::find(std::begin(_children), std::end(_children), child);
        assert(it != std::end(_children));
        _children.erase(it);
        child->removed_from_container(this);
    }

    template<class Config, bool With_layout>
    auto Abstract_container<Config, With_layout>::child_at(const Position &pos) -> Widget<Config, With_layout>*
    {
        auto child = std::find_if(std::begin(_children), std::end(_children), [&](auto child) { return child->rectangle().contains(pos); });

        return child != std::end(_children) ? *child : nullptr;
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::init_children_resources()
    {
        for (auto child : children())
        {
            child->init();
        }
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_motion(const Position &pos)
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

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_button(const Position &pos, int button, Key_state state)
    {
        auto child = child_at(pos);

        if (child) child->mouse_button(pos - child->position(), button, state);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_click(const Position &pos, int button, int count)
    {
        auto child = child_at(pos);

        if (child) child->mouse_click(pos - child->position(), button, count);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_wheel(const Position &dist)
    {
        if (_hovered_child) _hovered_child->mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_exit()
    {
        // We must propagate the exit to any currently hovered child
        if (_hovered_child)
        {
            _hovered_child->mouse_exit();
            _hovered_child = nullptr;
        }
    }

} // ns cppgui