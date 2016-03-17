#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool WithLayout>
    inline void Abstract_container<Config, WithLayout>::_add_child(Widget<Config, WithLayout> *child)
    {
        _children.push_back(child);
    }

    template<class Config, bool WithLayout>
    auto Abstract_container<Config, WithLayout>::child_at(const Position &pos) -> Widget<Config, WithLayout>*
    {
        Widget<Config, WithLayout> *target = nullptr;

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

    template<class Config, bool WithLayout>
    void Abstract_container<Config, WithLayout>::handle_mouse_motion(const Position &pos)
    {
        Widget<Config, WithLayout> *hovered = child_at(pos);

        if (hovered != _hovered_child)
        {
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }
    }

    template<class Config, class Update_handler, bool WithLayout>
    void Abstract_container<Config, WithLayout>::child_invalidated(Widget<Config, WithLayout>*)
    {
        static_cast<Config::Container_update_handler*>(this)->child_invalidated(child);
    }

    template<class Config, bool WithLayout>
    inline void Container_basic<Config, WithLayout>::add_child(Widget<Config, WithLayout> *child)
    {
        _add_child(child);

        static_cast<Config::Container_update_handler*>(this)->child_added(child);
        static_cast<Config::Widget_update_handler*>(child)->added_to_container(this);
    }

    template <class Config, class Update_handler, bool WithLayout>
    void Container_basic<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        Widget<Config, WithLayout> *hovered = child_at(pos);

        if (hovered != _hovered_child)
        {
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }
    }

    template <class Config, bool WithLayout>
    inline void Container_basic<Config, WithLayout>::update_render_resources(Renderer *r)
    {
        for (auto& child : _children)
        {
            child->update_render_resources(r);
        }
    }

    template <class Config, bool WithLayout>
    void Container_basic<Config, WithLayout>::render(Renderer *r, const Position & offset)
    {
        auto pos = offset + position();

        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

} // ns cppgui