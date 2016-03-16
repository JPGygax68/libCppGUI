#pragma once

#include "./Widget.hpp"

namespace cppgui {

    /** Container, without layouting.
     */
    template <class Config, bool WithLayout>
    class Container_basic: public Config::Container_base_update_handler, public Widget<Config, WithLayout> {
    public:
        void add_child(Widget *);

        void mouse_motion(const Position &) override;

        void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &offset) override;

    private:
        std::vector<Widget*>    _children;
        Widget                 *_hovered_child = nullptr;
    };

    template <class Config>
    class Container_full : public Container_basic<Config, true>, public Widget_layouter {
    public:
        //auto minimal_size() -> Extents override;
        //void layout() override;
    };

    template <class Config, bool WithLayout>
    class Container : public std::conditional<WithLayout, Container_full<Config>, Container_basic<Config, false>>::type {};

} // ns cppgui