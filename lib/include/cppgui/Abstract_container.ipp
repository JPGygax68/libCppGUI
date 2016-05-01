#pragma once

#include <algorithm>

#include "./Abstract_container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::child_has_obtained_focus(Widget_t *child)
    {
        if (_focused_child)
        {
            _focused_child->loosing_focus(); // TODO: support veto-ing loss of focus ?
        }

        _focused_child = child;

        /* REMOVED: notification originates from newly focused child -> no need to call back
        if (_focused_child)
        {
            _focused_child->gained_focus();
        }
        */
    }

    template <class Config, bool With_layout>
    inline void Abstract_container<Config, With_layout>::add_child(Widget_t *child)
    {
        _children.push_back(child);
        child->added_to_container(this);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::remove_child(Widget_t *child)
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

        auto it = std::find(std::begin(_children), std::end(_children), child);
        assert(it != std::end(_children));
        _children.erase(it);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::remove_all_children()
    {
        for (auto child: _children)
        {
            remove_child(child);
        }
    }

    template<class Config, bool With_layout>
    auto Abstract_container<Config, With_layout>::child_at(const Point &pos) -> Widget<Config, With_layout>*
    {
        auto child = std::find_if(std::begin(_children), std::end(_children), [&](auto child) { 
            return child->visible() && child->rectangle().contains(pos); 
        });

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
    void Abstract_container<Config, With_layout>::render_children(Canvas_t *cv, const Point & offs)
    {
        for (auto& child : children())
        {
            if (child->visible()) 
            {
                child->render(cv, offs);
            }
        }
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_motion(const Point &pos)
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
    void Abstract_container<Config, With_layout>::container_mouse_button(const Point &pos, int button, Key_state state)
    {
        auto child = child_at(pos);

        if (child) child->mouse_button(pos - child->position(), button, state);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_click(const Point &pos, int button, int count)
    {
        auto child = child_at(pos);

        if (child) child->mouse_click(pos - child->position(), button, count);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_mouse_wheel(const Point &dist)
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

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::container_text_input(const char32_t *text, size_t size)
    {
        if (_focused_child)
        {
            _focused_child->text_input(text, size);
        }
    }

    template<class Config, bool With_layout>
    bool Abstract_container<Config, With_layout>::container_key_down(const Keycode &key)
    {
        if (_focused_child)
        {
            return _focused_child->handle_key_down(key);
        }
        else
            return true;
    }

    template<class Config, bool With_layout>
    template<class Pred>
    auto Abstract_container<Config, With_layout>::scan_children_forward(Index from, Pred pred) -> Index
    {
        for (auto i = from; i < (Index) _children.size(); i ++)
        {
            if (pred(_children[i])) return i;
        }

        return (Index) _children.size();
    }

    template<class Config, bool With_layout>
    template<class Pred>
    auto Abstract_container<Config, With_layout>::scan_children_backward(Index from, Pred pred) -> Index
    {
        for (Index i = from; i >= 0; i--)
        {
            if (pred(_children[i])) return i;
        }

        return - 1;
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    void Abstract_container__Layouter<Config, true>::init_children_layout()
    {
        for (auto child : p()->children())
        {
            child->init_layout();
        }
    }

    template<class Config>
    inline void Abstract_container__Layouter<Config, true>::layout_children()
    {
        // TODO: this algorithm, and the whole method, will probably become obsolete as real
        //  layouting gets implemented

        for (auto child : p()->children())
        {
            // EXPERIMENTAL: obtain minimum size and extend accordingly
            auto min_ext = child->get_minimal_size(), cur_ext = child->extents();
            if (child->extents().w == 0 && child->extents().h == 0)
            {
                child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
            }
            child->layout();
        }
    }

} // ns cppgui
