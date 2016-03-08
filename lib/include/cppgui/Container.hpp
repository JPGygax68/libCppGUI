#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Renderer>
    class Container_min: public Widget<Renderer> {
    public:
        void add_child(Widget<Renderer> *);

    private:
        std::vector<Widget<Renderer>*> _children;
    };

    /*
    template<class Renderer>
    class Container_full : public Widget<Renderer>, public Layouter {
    public:
        auto minimal_size() -> Extents override;
        void layout() override;
    };

    template <class Renderer, bool WithLayout = true>
    class Container_full : public std::conditional<WithLayout, Container_full, Container_min> {};

    template<class Renderer>
    inline void Container_min<Renderer>::add_child(Widget<Renderer> *child)
    {
        _children.push_back(child);
    }
    */

} // ns cppgui