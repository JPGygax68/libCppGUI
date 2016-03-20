#pragma once

#include "./Widget.hpp"

namespace cppgui {

    /** Container functionality (ability to contain Widgets).
     */
    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_updater<Nil_struct>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;

        auto& children() { return _children; }

    protected:
        void _add_child(Widget_t *);
        auto child_at(const Position &) -> Widget_t *;

        /** The handle_mouse_xxxx() methods are intended as "delegates" to be called
            from "real" containers (i.e. descendants of Container<>).            
         */
        void handle_mouse_motion(const Position &);
        void handle_mouse_click(const Position &, int button, int count);

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
    };

    template <class Config, bool With_layout> struct Container_layouter;

    template <class Config, bool With_layout>
    struct Container_layouter {

        CPPGUI_DEFINE_ASPECT(Aspect) { };
    };


    template <class Config, bool With_layout>
    class Container: 
        public Container_layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >,
        public Abstract_container<Config, With_layout>
    {
    public:
        using Renderer = typename Config::Renderer;
        template <class Aspect_parent> using Layouter_t = Container_layouter::Aspect<Aspect_parent>;

        void add_child(Widget<Config, With_layout> *); // TODO: really need to override ?

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;

        void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;
    };

    template <class Config, bool With_layout>
    struct Default_abstract_container_updater {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;

            virtual void child_invalidated(Widget_t *);
        };
    };

} // ns cppgui