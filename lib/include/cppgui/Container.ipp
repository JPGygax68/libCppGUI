#include "./Container.hpp"

namespace cppgui {

    template <class Config>
    inline void Container_min<Config>::add_child(Widget<Config> *child)
    {
        _children.push_back(child);
    }

    template <class Config>
    inline void Container_min<Config>::mouse_motion(const Position &pos)
    {
        Widget<Config> *hovered = nullptr;

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

    template <class Config>
    void Container_min<Config>::update_resources(Renderer *r)
    {
        for (auto& child: _children)
        {
            child->update_resources(r);
        }
    }

    template <class Config>
    void Container_min<Config>::render(Renderer *r, const Position & offset)
    {
        auto pos = offset + position();

        for (auto& child : _children)
        {
            child->render(r, pos);
        }
    }

} // ns cppgui