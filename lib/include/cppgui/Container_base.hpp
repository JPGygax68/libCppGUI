#pragma once

#include "./Widget.hpp"
#include "./Abstract_container.hpp"

namespace cppgui {

    // Forward declarations

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget.
     */
    template <class Config, bool With_layout>
    class Container_base: 
        public Widget<Config, With_layout>, 
        public Config::template Container_base__Container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Abstract_container_t = typename Abstract_container<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;

        void init() override;

        using Abstract_container_t::add_child;
        using Abstract_container_t::remove_child;
        using Abstract_container_t::remove_all_children;

        bool container_has_focus() override { return has_focus(); }

        void container_take_focus(Widget_t *) override;
        void gained_focus() override;
        void loosing_focus() override;

        void mouse_motion(const Point &) override;
        void mouse_button(const Point &, int button, Key_state) override;
        void mouse_click(const Point &, int button, int count) override;
        void mouse_wheel(const Vector &) override;
        void text_input(const char32_t *, size_t) override;
        void key_down(const Keycode &) override;

        void mouse_exit() override;

        /** Handle key_down event that has "bubbled" back up from the child it was send to.
         */
        void child_key_down(const Keycode &key) override;

        void render(Canvas_t *, const Point &) override;

    protected:

        bool handle_key_down(const Keycode &) override;

        // Actions (return value indicates success)
        bool cycle_focus_forward ();
        bool cycle_focus_backward();
    };

    // Container_base_updater aspect

    template <class Config, bool With_layout, class Parent>
    struct Default__Container_base__Container_updater: public Parent 
    {
        class Container_base_t: public Container_base<Config, true> { friend struct Default__Container_base__Container_updater; };
        using Widget_t = Widget<Config, true>;

        auto p() { return static_cast<Container_base_t*>(static_cast<Container_base<Config, true>*>(this)); }

        void child_invalidated(Widget_t *) override;

        auto container_root_widget() { return p()->root_widget(); }
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_CONTAINER_BASE(Config, With_layout) \
    template cppgui::Container_base<Config, With_layout>;
