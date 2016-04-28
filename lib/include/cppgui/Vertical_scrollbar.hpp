#pragma once

#include "./Container.hpp"
#include "./Glyph_button.hpp"
//#include "./Drag_controller.hpp"

namespace cppgui {

    // Forward declarations

    template <class Config, bool With_layout>
    struct Vertical_scrollbar__Layouter {
        template <class Aspect_parent> struct Aspect;
    };

    template <class Config, bool With_layout> class Vertical_scrollbar_thumb;

    // Class declaration

    template<class Config, bool With_layout>
    class Vertical_scrollbar: public Vertical_scrollbar__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Container_t = typename Container<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Thumb_t = typename Vertical_scrollbar_thumb<Config, With_layout>;

        //using Position_change_handler = std::function<void(const Fraction<> &)>;
        using Navigation_handler = std::function<void(Navigation_unit, Position initial_pos, const Fraction<int> &delta)>; //, bool ending)>;

        Vertical_scrollbar();

        //void on_position_change(Position_change_handler);
        void on_navigation(Navigation_handler);

        /** Defines the range to be covered by the scrollbar, consisting of the "full"
            range and the "shown" range, visually represented by the "slide" between
            the up/down buttons and the length of the thumb, respectively.
         */
        void define_range(Position full, Position shown, Position element = 0);

        void init() override;

        void mouse_button(const Point &, int button, Key_state) override;
        void mouse_motion(const Point &) override;
        void mouse_wheel(const Vector & ) override;
        void mouse_exit() override;

        void render(Canvas_t *, const Point &offset) override;

        void change_range(Position full, Position shown, Position element = 0);
        auto range_length() const { return _full_length; }
        auto covered_length() const { return _covered_length; }

        auto current_position() -> Position; // TODO: use rational number instead ?
        void change_position(Position); // TODO: assert() against calling this while not at end of navigation ?

        // Actions
        void page_up();
        void page_down();

    protected:
        using Glyph_button_t = typename Glyph_button<Config, With_layout>;

        friend Thumb_t;
        
        void move_thumb_to(Position);
        void move_by_elements(int delta);
        void recalc_thumb();
        void clip_thumb_pos();
        //void notify_position_change();
        void notify_drag_navigation(Position_delta);

        Glyph_button_t          _up_btn, _down_btn;
        //Position_change_handler _on_position_change;
        Navigation_handler      _nav_handler;
        Position                _full_length = 0, _covered_length = 0, _element_length = 0;

        Range                   _sliding_range;
        Rectangle               _thumb_rect;

        bool                    _thumb_hovered = false;
        Position                _thumb_drag_start_pos;
        Position                _thumb_drag_anchor_pos;
        Position                _drag_start_pos;
        bool                    _dragging_thumb = false;
    };

    // Layouter aspect

    template <class Config>
    struct Vertical_scrollbar__Layouter<Config, true> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            // Layouter contract

            //void init_layout() override;
            //auto get_minimal_size() -> Extents override;
            void layout() override;

            // Extra capabilities coming with layouting
            // TODO

        protected:
            class Vertical_scrollbar_t: public Vertical_scrollbar<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Vertical_scrollbar_t*>(this); }
        };
    };

    #ifdef THUMB_AS_WIDGET

    // Thumb --------------------------------------------------------

    template <class Config, bool With_layout> struct Vertical_scrollbar_thumb__Layouter {
        template<class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    template<class Config, bool With_layout>
    class Vertical_scrollbar_thumb: public Vertical_scrollbar_thumb__Layouter<Config, With_layout>::template Aspect< Widget<Config, With_layout> >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Vertical_scrollbar_t = typename Vertical_scrollbar<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        void mouse_button(const Point &, int button, Key_state) override;
        void mouse_motion(const Point &) override;
        //void mouse_exit() override;

        void render(Canvas_t *, const Point &offset) override;

    protected:
        using Drag_controller_t = Drag_controller<Config, With_layout>;

        auto scrollbar() { return static_cast<Vertical_scrollbar_t*>(container()); }

        Drag_controller_t       _drag_ctl;
        Position                  _drag_start_pos;
    };

    // Thumb layouter aspect

    template <class Config>
    struct Vertical_scrollbar_thumb__Layouter<Config, true> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            // Layouter contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Extra capabilities coming with layouting
            // TODO

        protected:
            class Vertical_scrollbar_thumb_t: public Vertical_scrollbar_thumb<Config, true> { friend struct Aspect; };
            using Vertical_scrollbar_t = Vertical_scrollbar<Config, true>;

            auto p() { return static_cast<Vertical_scrollbar_thumb_t*>(this); }
        };
    };

    #endif

} // ns cppgui
