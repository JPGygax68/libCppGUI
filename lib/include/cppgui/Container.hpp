#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Renderer>
    class Container_min: public Widget<Renderer> {
    public:
        void add_child(Widget<Renderer> *);

        void mouse_motion(const Position &) override;

        void update_resources(Renderer *) override;
        void render(Renderer *, const Position &offset) override;

    private:
        std::vector<Widget<Renderer>*> _children;
        Widget                         *_hovered_child = nullptr;
    };

    template <class Renderer>
    class Container_full : public Container_min<Renderer>, public Widget_layouter {
    public:
        //auto minimal_size() -> Extents override;
        //void layout() override;
    };

    template <class Renderer, bool WithLayout = true>
    class Container : public std::conditional<WithLayout, Container_full<Renderer>, Container_min<Renderer>>::type {};

} // ns cppgui