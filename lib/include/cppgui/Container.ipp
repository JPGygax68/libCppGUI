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
    }

    template<class Config, bool With_layout>
    void Abstract_container<Config, With_layout>::handle_mouse_motion(const Position &pos)
    {
        Widget<Config, With_layout> *hovered = child_at(pos);

        if (hovered != _hovered_child)
        {
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_motion(const Position &pos)
    {
        Widget<Config, With_layout> *hovered = child_at(pos);

        if (hovered != _hovered_child)
        {
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }
    }

    template <class Config, bool With_layout>
    inline void Container<Config, With_layout>::update_render_resources(Renderer *r)
    {
        for (auto& child : _children)
        {
            child->update_render_resources(r);
        }
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::render(Renderer *r, const Position & offset)
    {
        auto pos = offset + position();

        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    template<class Config, bool With_layout>
    template<class Next_aspects>
    inline void Default_abstract_container_updater<Config, With_layout>::Aspect<Next_aspects>::child_invalidated(Widget_t *)
    {
        // TODO
    }

} // ns cppgui