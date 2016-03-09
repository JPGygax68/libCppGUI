#include "./Container.hpp"

namespace cppgui {

    template<class Renderer>
    void Container_min<Renderer>::add_child(Widget<Renderer> *child)
    {
        _children.push_back(child);
    }

    template<class Renderer>
    inline void Container_min<Renderer>::mouse_motion(const Position &)
    {
        std::cerr << "Container<Renderer>::mouse_motion(): TODO!" << std::endl;

        // TODO: detect mouse in / out for children; send to hovered child
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