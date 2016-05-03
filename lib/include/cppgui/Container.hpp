#pragma once

#include "./Widget.hpp"
#include "./Abstract_container.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    enum class Layout_type { none, 
        header_content, 
        content_footer, 
        head_content,           // horizontal: fixed-width head, followed by stretching content TODO: better name! 
        content_tail,           // opposite of head_content
        stack,
        end
    };

    // Forward-declarations

    template <class Config, bool With_layout>
    struct Container__Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget. At the present (2016-05-03), it also offers 
        a choice of layouting algorithms, but that is bad design and will be removed.
        The same is true of its built-in support for borders, which is probably not needed
        at this level - it would probably be better to move that feature to the Scrollbox
        container specialization (and possibly other, yet-to-defined specializations).
     */
    template <class Config, bool With_layout>
    class Container: 
        public Container__Layouter<Config, With_layout>::template Aspect < Widget<Config, With_layout> >, 
        public Config::template Container_Container_updater< Abstract_container<Config, With_layout> >
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
        auto paper_margin() -> unsigned int { return 2; } // TODO: remove (or move to Stack<>)

        bool handle_key_down(const Keycode &) override;

        // Actions (return value indicates success)
        bool cycle_focus_forward ();
        bool cycle_focus_backward();

        // TODO: this is TEMPORARY, remove! (or replace the whole Container<> class)
        //Rectangle               _inner_rect;
    };

    // Container_updater aspect

    template <class Config, bool With_layout>
    struct Default__Container__Container_updater {

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
    struct Container__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            using Widget_t = typename Widget<Config, true>;
            class Container_t: public Container<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Container_t*>(this); }

            // Layout contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            auto get_preferred_size() -> Extents override;
            void layout() override;

            // Specific interface

            void set_layout_type(Layout_type type ) { _layout_type = type; }

            void insert_child(Widget_t *); // TODO: find a better name OR support insertion index
            void drop_child(Widget_t *);

            void set_spacing(Length spacing) { _spacing = spacing; }

        private:
            Layout_type     _layout_type = Layout_type::none;
            Length          _spacing = 0;
            //Extents         _comp_min_size  = { 0, 0 };
            //Extents         _comp_pref_size = { 0, 0 };
        };
    };

    // Nil implementation of the Layouter aspect

    template <class Config>
    struct Container__Layouter<Config, false> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            // void recalc_sizes() {}
        };
    };
        
} // ns cppgui