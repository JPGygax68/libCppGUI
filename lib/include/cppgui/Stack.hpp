#pragma once

#include "./Container.hpp"
#include "./Button.hpp"

namespace cppgui {

    // Forward declarations

    template <class Config, bool With_layout>
    struct Stack__Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout> class Inner_stack;

    // Stack container widget

    template<class Config, bool With_layout>
    class Stack: public Stack__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Widget_t = Widget<Config, With_layout>;
        using Button_t = typename Button<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;

        Stack();

        void add_child(Widget_t *);

        void render(Canvas_t *, const Point &offs) override;

        void gained_focus() override;

    protected:
        using Inner_stack_t = typename Inner_stack<Config, With_layout>;

        bool handle_key_down(const Keycode &) override;

        static auto glyph_font() -> const Rasterized_font *;
        auto compute_children_total_size() -> Extents;

        Button_t        _up_btn, _down_btn;
        Inner_stack_t   _inner_stack;
    };

    // Layouter aspect

    template <class Config>
    struct Stack__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            class Stack_t: public Stack<Config, true> { friend struct Aspect; };
            using Container_t = Container<Config, true>;

            auto p() { return static_cast<Stack_t*>(this); }

            //void init_layout() override;  // TODO: provide our own that does not use a layouting algorithm
                                            // or better yet, rewrite the whole Stack container
            auto get_minimal_size() -> Extents override;
            void layout() override;

        private:
            Extents _min_exts;
        };
    };

    // "Inner" stack ------------------------------------------------

    // TODO: move to .ipp file ?

    template <class Config, bool With_layout>
    struct Inner_stack__Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout>
    class Inner_stack: public Inner_stack__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Container_t = typename Container<Config, With_layout>;
        using Canvas_t = typename Container_t::Canvas_t;

        Inner_stack() { set_background_color({1, 1, 0.7f, 1}); } // TODO: get it from a source

        void init() override;

        void mouse_motion(const Point &) override;
        void mouse_click(const Point &, int button, int count) override;
        // TODO: mouse_button & any other events that carry a mouse position
        void mouse_wheel(const Vector &dist) override;

        void render(Canvas_t *, const Point &) override;

    protected:
        friend class Stack<Config, With_layout>;

        // Actions
        void scroll_down();
        void scroll_up  ();

        // Hooks
        void child_has_obtained_focus(Widget_t *child) override;

        // Helpers
        void bring_child_into_view(Widget_t *);

        unsigned    _first_visible_item = 0;
        int         _children_offset = 0;
        Point    _last_mouse_pos = {};
    };

    // Layouter aspect

    template <class Config>
    struct Inner_stack__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            using Parent_t = typename Aspect_parent;
            class Inner_stack_t: public Inner_stack<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Inner_stack_t*>(this); }

            auto get_minimal_size() -> Extents override;

            /*
            //void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;
            */
        };
    };

} // ns cppgui