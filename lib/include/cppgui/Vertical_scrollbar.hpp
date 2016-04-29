#pragma once

#include "./Container.hpp"
#include "./Glyph_button.hpp"
//#include "./Drag_controller.hpp"

namespace cppgui {

    // Forward declarations

    template <class Impl, class Config, bool With_layout>
    struct Vertical_scrollbar__Layouter {
        template <class Aspect_parent> struct Aspect;
    };

    // Base class ===================================================

    // Class declaration

    template<class Impl, class Config, bool With_layout>
    class Vertical_scrollbar_base: public Vertical_scrollbar__Layouter<Impl, Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Container_t = typename Container<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        using Navigation_handler = std::function<void(Navigation_unit, Position initial_pos, const Fraction<int> &delta)>; //, bool ending)>;

        Vertical_scrollbar_base();

        /** Defines the range to be covered by the scrollbar, consisting of the "full"
            range and the "shown" range, visually represented by the "slide" between
            the up/down buttons and the length of the thumb, respectively.
         */
        void define_range(Length full, Length shown);

        void init() override;

        void mouse_button(const Point &, int button, Key_state) override;
        void mouse_motion(const Point &) override;
        void mouse_wheel(const Vector & ) override;
        void mouse_exit() override;

        void render(Canvas_t *, const Point &offset) override;

        void change_range(Length range, Length thumb);
        auto range() const { return _range; }
        auto thumb_length() const { return _thumb_length; }

        auto current_position() -> Position; // TODO: use rational number instead ?
        void change_position(Position); // TODO: assert() against calling this while not at end of navigation ?

    protected:
        using Glyph_button_t = typename Glyph_button<Config, With_layout>;

        // To be implemented in derived class (via CRTP)
        void move_by_page(int delta) { static_assert(false); }
        void move_by_elements(int delta) { static_assert(false); }
        void move_by_fraction(Position initial_pos, const Fraction<int> &delta) { static_assert(false); }

        void move_thumb_to(Position);
        void recalc_thumb();
        void clip_thumb_pos();
        void notify_drag_navigation(Position_delta);

        Glyph_button_t          _up_btn, _down_btn;
        Position                _range = 0, _thumb_length = 0;

        Range                   _sliding_range;
        Rectangle               _thumb_rect;

        bool                    _thumb_hovered = false;
        Position                _thumb_drag_start_pos;
        Position                _thumb_drag_anchor_pos;
        Position                _drag_start_pos;
        bool                    _dragging_thumb = false;
    };

    // Layouter aspect

    template <class Impl, class Config>
    struct Vertical_scrollbar__Layouter<Impl, Config, true> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            // Layouter contract

            //void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Extra capabilities coming with layouting
            // TODO

        protected:
            class Vertical_scrollbar_base_t: public Vertical_scrollbar_base<Impl, Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Vertical_scrollbar_base_t*>(this); }
        };
    };

    // Customizable specialization ==================================

    template<class Config, bool With_layout>
    class Custom_vertical_scrollbar: public Vertical_scrollbar_base<Custom_vertical_scrollbar<Config, With_layout>, Config, With_layout>
    {
    public:
        void on_navigation(Navigation_handler);

    protected:
        friend class Vertical_scrollbar_base<Custom_vertical_scrollbar, Config, With_layout>;

        void move_by_page(int delta);
        void move_by_elements(int delta);
        void move_by_fraction(Position initial_pos, const Fraction<int> &delta);

        Navigation_handler      _nav_handler;
    };

    // Standalone specialization ===================================

    template<class Config, bool With_layout>
    class Vertical_scrollbar: public Vertical_scrollbar_base<Vertical_scrollbar<Config, With_layout>, Config, With_layout>
    {
    public:
        using Position_change_handler = std::function<void(Position)>;

        void on_position_change(Position_change_handler);

        void move_by_page(int delta);
        void move_by_elements(int delta);
        void move_by_fraction(Position initial_pos, const Fraction<int> &delta);

    protected:
        void notify_position_change();

        Position_change_handler _on_pos_chng;
    };

} // ns cppgui

