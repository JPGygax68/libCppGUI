#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config>
    class Container_min: public Widget<Config> {
    public:
        void add_child(Widget<Config> *);

        void mouse_motion(const Position &) override;

        void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &offset) override;

    private:
        std::vector<Widget<Config>*> _children;
        Widget                         *_hovered_child = nullptr;
    };

    template <class Config>
    class Container_full : public Container_min<Config>, public Widget_layouter {
    public:
        //auto minimal_size() -> Extents override;
        //void layout() override;
    };

    template <class Config, bool WithLayout = true>
    class Container : public std::conditional<WithLayout, Container_full<Config>, Container_min<Config>>::type {};

} // ns cppgui