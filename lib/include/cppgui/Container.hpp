#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout> struct Abstract_container_Layouter {};

    /** Container functionality (ability to contain Widgets).
     */
    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_Container_updater<Nil_struct>,
        public Abstract_container_Layouter<Config, With_layout>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;

        void add_child(Widget_t *);

        auto& children() { return _children; }

    protected:
        auto child_at(const Position &) -> Widget_t *;

        void init_children_resources();

        /** The handle_mouse_xxxx() methods are intended as "delegates" to be called
            from "real" containers (i.e. descendants of Container<>).            
         */
        void container_mouse_motion(const Position &);
        void container_mouse_button(const Position &, int button, Key_state);
        void container_mouse_click(const Position &, int button, int count);
        void container_mouse_exit();

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
    };

    template <class Config, bool With_layout> class Container;

    // Container_updater aspect
    
    // IMPORTANT! This is *different* from the "Updater" aspect, which belongs to Widgets!

    template <class Config, bool With_layout>
    struct Default_Abstract_container_Container_updater {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            using Widget_t = Widget<Config, With_layout>;
            class Container_t: public Container<Config, true> { friend struct Aspect; };
            using Root_widget_t = Root_widget<Config, With_layout>;

            auto p() { return static_cast<Container_t*>(this); }

            virtual void child_invalidated(Widget_t *) = 0;

            virtual auto container_root_widget() -> Root_widget_t * = 0;
        };
    };

    template <class Config> struct Abstract_container_Layouter<Config, true> {

        class Container_t: public Container<Config, true> { friend struct Aspect; };

        auto p() { return static_cast<Container_t*>(this); }

        void init_children_layout();
        void layout_children();
    };

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

        void mouse_exit() override;

        //void update_render_resources(Renderer *) override;
        void render(Canvas_t *, const Position &) override;
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
            //void layout() override;
        };
    };

} // ns cppgui