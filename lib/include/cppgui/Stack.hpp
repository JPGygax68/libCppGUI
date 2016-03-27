#pragma once

#include "./Container.hpp"
#include "./Button.hpp"

namespace cppgui {

    // Forward declarations

    template <class Config, bool With_layout>
    struct Stack_layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout> class Inner_stack;

    // Stack container widget

    template<class Config, bool With_layout>
    class Stack: public Stack_layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Widget_t = Widget<Config, With_layout>;
        using Button_t = typename Button<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        Stack();

        void add_child(Widget_t *);

        void render(Canvas_t *, const Position &offs) override;

    protected:
        using Inner_stack_t = typename Inner_stack<Config, With_layout>;

        auto border_color() -> Color;
        auto border_width() -> int { return 1; }
        static auto glyph_font() -> const Rasterized_font *;
        auto compute_children_total_size() -> Extents;

        Button_t        _up_btn, _down_btn;
        Inner_stack_t   _inner_stack;
    };

    // Layouter aspect

    template <class Config>
    struct Stack_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            class Stack_t: public Stack<Config, true> { friend struct Aspect; };
            using Container_t = Container<Config, true>;

            auto p() { return static_cast<Stack_t*>(this); }

            //void compute_sizes();

            //void init_layout() override;
            auto minimal_size() -> Extents override;
            void layout() override;

        private:
            Extents _min_exts;
        };
    };

    // "Inner" stack ------------------------------------------------

    // TODO: move to .ipp file ?

    template <class Config, bool With_layout>
    struct Inner_stack_Layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template <class Config, bool With_layout>
    class Inner_stack: public Inner_stack_Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = typename Container<Config, With_layout>;
        using Canvas_t = typename Container_t::Canvas_t;

        void init() override;

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;
        // TODO: mouse_button & any other events that carry a mouse position
        void mouse_wheel(const Position &dist) override;

        void render(Canvas_t *, const Position &) override;

    protected:
        friend class Stack<Config, With_layout>;

        // Actions
        void scroll_up  ();
        void scroll_down();

        unsigned    _first_visible_item = 0;
        int         _children_offset = 0;
        Position    _last_mouse_pos = {};
    };

    // Layouter aspect

    template <class Config>
    struct Inner_stack_Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            class Inner_stack_t: public Inner_stack<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Inner_stack_t*>(this); }

            //void compute_sizes();

            //void init_layout() override;
            auto minimal_size() -> Extents override { return {0, 0}; } // TODO: better ?
            void layout() override {}
        };
    };

} // ns cppgui