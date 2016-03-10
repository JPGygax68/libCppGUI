#include "./Container.hpp"

namespace cppgui {

    template<class Renderer>
    void Container_min<Renderer>::add_child(Widget<Renderer> *child)
    {
        _children.push_back(child);
    }

    template<class Renderer>
    inline void Container_min<Renderer>::mouse_motion(const Position &pos)
    {
        Widget<Renderer> *hovered = nullptr;

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

    template<class Renderer>
    void Container_min<Renderer>::update_resources(Renderer *r)
    {
        for (auto& child: _children)
        {
            child->update_resources(r);
        }
    }

    template<class Renderer>
    void Container_min<Renderer>::render(Renderer *r, const Position & offset)
    {
        auto pos = offset + position();

        for (auto& child : _children)
        {
            child->render(r, pos);
        }
    }

} // ns cppgui