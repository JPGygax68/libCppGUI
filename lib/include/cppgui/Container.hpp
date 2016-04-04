#pragma once

#include "./Widget.hpp"
#include "./Abstract_container.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    // Forward-declare layouter aspect

    template <class Config, bool With_layout>
    struct Container_Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout>
    class Container: 
        public Container_Layouter<Config, With_layout>::template Aspect < Widget<Config, With_layout> >, 
        public Config::template Container_Container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        //template <class Aspect_parent> using Layouter_t = Container_layouter::Aspect<Aspect_parent>;

        void init() override;

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;
        void mouse_wheel(const Position &) override;

        void mouse_exit() override;

        //void update_render_resources(Renderer *) override;
        void render(Canvas_t *, const Position &) override;

    protected:
        auto background_color() -> Color { return {1, 1, 0.7f, 1}; }
        auto paper_margin() -> unsigned int { return 2; }
    };

    template <class Config, bool With_layout> struct Default_Container_Container_updater {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            class Container_t: public Container<Config, true> { friend struct Aspect; };
            using Widget_t = Widget<Config, true>;

            auto p() { return static_cast<Container_t*>(this); }

            void child_invalidated(Widget_t *) override;

            auto container_root_widget() { return p()->root_widget(); }
        };
    };

    // Layouter aspect

    template <class Config>
    struct Container_Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            class Container_t: public Container<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Container_t*>(this); }

            //void compute_sizes();

            void init_layout() override;
            //auto minimal_size() -> Extents override;
            void layout() override;
        };
    };

} // ns cppgui