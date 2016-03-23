#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout> struct Abstract_container_layouter {};

    /** Container functionality (ability to contain Widgets).
     */
    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_updater<Nil_struct>,
        public Abstract_container_layouter<Config, With_layout>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;

        auto& children() { return _children; }

    protected:
        void _add_child(Widget_t *);
        auto child_at(const Position &) -> Widget_t *;

        void init_children();

        /** The handle_mouse_xxxx() methods are intended as "delegates" to be called
            from "real" containers (i.e. descendants of Container<>).            
         */
        void handle_mouse_motion(const Position &);
        void handle_mouse_button(const Position &, int button, Key_state);
        void handle_mouse_click(const Position &, int button, int count);

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
    };

    template <class Config, bool With_layout> class Container;

    template <class Config, bool With_layout>
    struct Default_abstract_container_updater {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            virtual void child_invalidated(Widget_t *);

            virtual auto _root_widget() -> Root_widget_t * = 0;
        };
    };

    template <class Config> struct Abstract_container_layouter<Config, true> {

        void layout_children();
    };


    // Container base class: descended from Widget

    template <class Config, bool With_layout>
    class Container: public Widget<Config, With_layout>, public Abstract_container<Config, With_layout>
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        //template <class Aspect_parent> using Layouter_t = Container_layouter::Aspect<Aspect_parent>;

        void add_child(Widget<Config, With_layout> *); // TODO: really need to override ?

        void init() override;

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;

        //void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;
    };

} // ns cppgui