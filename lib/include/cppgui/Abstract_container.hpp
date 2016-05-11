#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout> struct Abstract_container__Layouter {};

    /** Container functionality (ability to contain Widgets).
    */
    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_Container_updater<Nil_struct>,
        public Abstract_container__Layouter<Config, With_layout>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Keyboard = typename Config::Keyboard;
        using Keycode  = typename Keyboard::Keycode;

        Abstract_container(): _hovered_child { nullptr } {}

        void set_initial_focus(Widget_t *);

        auto& children() { return _children; }

        /** Called when a key_down event could not be handled by the child it was sent to
            (from container_key_down()) and needs to "bubble" back up.
         */
        virtual void child_key_down(const Keycode &) = 0;

        virtual bool container_has_focus() = 0;       
        virtual void container_take_focus(Widget_t *);
        auto focused_child() -> Widget_t * { return _focused_child; }

        auto first_child() -> Widget_t * { assert(!_children.empty()); return _children.front(); }
        auto last_child () -> Widget_t * { assert(!_children.empty()); return _children.back (); }

        template<class Pred> auto scan_children_forward (Index from, Pred) -> Index;
        template<class Pred> auto scan_children_backward(Index from, Pred) -> Index;

    protected:

        void add_child(Widget_t *);
        // TODO: should removal methods be moved to optional aspect ?
        void remove_child(Widget_t *);
        void remove_all_children();

        auto child_at(const Point &) -> Widget_t *;

        void init_child_resources();
        void compute_child_views();

        void render_children(Canvas_t *, const Point &offs);

        /** The container_xxxx() methods are intended as "delegate" event handlers, to be 
            called from "real" containers (i.e. descendants of Container<>).            
        */
        void container_mouse_motion(const Point &);
        void container_mouse_button(const Point &, int button, Key_state);
        void container_mouse_click(const Point &, int button, int count);
        void container_mouse_wheel(const Point &dist);
        void container_mouse_exit();
        void container_text_input(const char32_t *, size_t);
        bool container_key_down(const Keycode &);

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
        Widget_t *_focused_child = nullptr;
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

    template <class Config> struct Abstract_container__Layouter<Config, true> {

        class Container_t: public Container<Config, true> { friend struct Aspect; };

        auto p() { return static_cast<Container_t*>(this); }

        void init_children_layout();
        void layout_children();
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_ABSTRACT_CONTAINER(Config, With_layout) \
    template cppgui::Abstract_container          <Config, With_layout>; \
    template cppgui::Abstract_container__Layouter<Config, With_layout>;