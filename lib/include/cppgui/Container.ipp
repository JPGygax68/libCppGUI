#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool WithLayout>
    inline void Container_basic<Config, WithLayout>::add_child(Widget *child)
    {
        _children.push_back(child);
        Config::Container_base_update_handler::child_added(child);
    }

    template <class Config, bool WithLayout>
    inline void Container_basic<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        Widget *hovered = nullptr;

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
    void Container_basic<Config, WithLayout>::update_render_resources(Renderer *r)
    {
        for (auto& child: _children)
        {
            child->update_render_resources(r);
        }
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