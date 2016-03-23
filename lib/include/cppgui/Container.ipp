#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    inline void Abstract_container<Config, With_layout>::_add_child(Widget<Config, With_layout> *child)
    {
        _children.push_back(child);
    }

    template<class Config, bool With_layout>
    auto Abstract_container<Config, With_layout>::child_at(const Position &pos) -> Widget<Config, With_layout>*
    {
        auto child = std::find_if(std::begin(_children), std::end(_children), [&](auto child) { return child->rectangle().contains(pos); });

        return child != std::end(_children) ? *child : nullptr;

        /*
        Widget<Config, With_layout> *target = nullptr;

        for (auto child : _children)
        {
            if (child->rectangle().contains(pos))
            {
                child->mouse_motion(pos - child->position());
                target = child;
            }
        }

        return target;
        */
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::init_children()
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

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children();

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

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_abstract_container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        // TODO
    }

    template<class Config>
    inline void Abstract_container_layouter<Config, true>::layout_children()
    {
        auto p = static_cast<Abstract_container<Config, true>*>(this);

        for (auto child : p->children())
        {
            // EXPERIMENTAL: obtain minimum size and extend accordingly
            auto min_ext = child->minimal_size(), cur_ext = child->extents();
            child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
            child->layout();
        }
    }

} // ns cppgui