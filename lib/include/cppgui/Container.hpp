#pragma once

#include "./Widget.hpp"

namespace cppgui {

    /** Container functionality (ability to contain and render Widgets).
     */
    template <class Config, bool WithLayout>
    class _Container: public Abstract_widget<Config, WithLayout> {
    public:

        auto& children() { return _children; }

        void mouse_motion(const Position &) override;

        void update_render_resources(Renderer *) override;

    protected:
        void _add_child(Widget<Config, WithLayout> *);

    private:
        std::vector<Widget<Config, WithLayout>*>   _children;
        Widget<Config, WithLayout>                *_hovered_child = nullptr;
    };

    /** Concrete Container (without layouting): combination of Widget and _Container.
     */
    template <class Config, bool WithLayout>
    class Container_basic: public Config::Container_update_handler, public Widget<Config, WithLayout> {
    public:

        void add_child(Widget<Config, WithLayout> *); // TODO: really need to override ?

        void render(Renderer *, const Position &offset) override;
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