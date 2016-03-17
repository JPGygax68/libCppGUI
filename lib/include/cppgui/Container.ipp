#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool WithLayout>
    inline void _Container<Config, WithLayout>::_add_child(Widget<Config, WithLayout> *child)
    {
        _children.push_back(child);
    }

    template <class Config, bool WithLayout>
    inline void _Container<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        Widget<Config, WithLayout> *hovered = nullptr;

        for (auto child : _children)
        {
            if (child->rectangle().contains(pos))
            {
                child->mouse_motion(pos - child->position());
                hovered = child;
            }
        }

        if (hovered != _hovered_child)
        {
            if (_hovered_child) _hovered_child->mouse_exit();
            if (hovered) hovered->mouse_enter();
            _hovered_child = hovered;
        }
    }

    template <class Config, bool WithLayout>
    inline void _Container<Config, WithLayout>::update_render_resources(Renderer *r)
    {
        for (auto& child: _children)
        {
            child->update_render_resources(r);
        }
    }

    template<class Config, bool WithLayout>
    inline void Container_basic<Config, WithLayout>::add_child(Widget<Config, WithLayout> *child)
    {
        _add_child(child);

        static_cast<Config::Container_update_handler*>(this)->child_added(child);
    }

    template <class Config, bool WithLayout>
    void Container_basic<Config, WithLayout>::render(Renderer *r, const Position & offset)
    {
        auto pos = offset + position();

        for (auto& child : _children)
        {
            child->render(r, pos);
        }
    }

} // ns cppgui