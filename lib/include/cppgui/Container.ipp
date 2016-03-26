#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    inline void Abstract_container<Config, With_layout>::add_child(Widget_t *child)
    {
        _children.push_back(child);
        child->added_to_container(this);
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
    void Abstract_container<Config, With_layout>::handle_mouse_motion(const Position &pos)
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
    void Abstract_container<Config, With_layout>::handle_mouse_button(const Position &pos, int button, Key_state state)
    {
        auto child = child_at(pos);

        if (child) child->mouse_button(pos - child->position(), button, state);
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::handle_mouse_click(const Position &pos, int button, int count)
    {
        auto child = child_at(pos);

        if (child) child->mouse_click(pos - child->position(), button, count);
    }

    /*
    template<class Config, bool With_layout>
    void Container<Config, With_layout>::add_child(Widget_t *child)
    {
        _add_child(child);
    }
    */

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children_resources();

        Widget_t::init();
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_motion(const Position &pos)
    {
        handle_mouse_motion(pos - position());
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        handle_mouse_click(pos - position(), button, count);
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::render(Renderer *r, const Position &offs)
    {
        auto pos = offs + position();

        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    // Updater aspect -----------------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_abstract_container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        // TODO
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    void Abstract_container_layouter<Config, true>::init_children_layout()
    {
        for (auto child : p()->children())
        {
            child->init_layout();
        }
    }

    template<class Config>
    inline void Abstract_container_layouter<Config, true>::layout_children()
    {
        for (auto child : p()->children())
        {
            // EXPERIMENTAL: obtain minimum size and extend accordingly
            auto min_ext = child->minimal_size(), cur_ext = child->extents();
            child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
            child->layout();
        }
    }

} // ns cppgui