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

        using Position_change_handler = std::function<void(const Fraction<> &)>;
        struct Range { int start, end; };

        Vertical_scrollbar();

        void on_position_change(Position_change_handler);

        void set_value_range(const Range &);
        void set_thumb_length(unsigned int);

        void init() override;

        void mouse_button(const Position &, int button, Key_state) override;
        void mouse_motion(const Position &) override;
        void mouse_wheel(const Position_delta & ) override;
        void mouse_exit() override;

        void render(Canvas_t *, const Position &offset) override;

        void change_value_range(const Range &);
        void change_thumb_length(unsigned int);

        auto current_position() -> Fraction<>;

        // Actions

        void page_up();
        void page_down();

    protected:
        using Glyph_button_t = typename Glyph_button<Config, With_layout>;

        friend Thumb_t;
        
        struct Sliding_range {
            Offset  start, end; // start and end of sliding range (in pixels)
            auto length() const { return static_cast<Offset>(end - start); }
        };

        void move_thumb_to(Offset);
        void recalc_thumb();
        void clip_thumb_pos();
        void notify_position_change();

        Glyph_button_t          _up_btn, _down_btn;
        Position_change_handler _on_position_change;
        Range                   _position_range = { 0, 100 };
        unsigned int            _thumb_length = 10; // TODO: better use a default of 1 here, or even 0

        Sliding_range           _sliding_range;
        Rectangle               _thumb_rect;

        bool                    _thumb_hovered = false;
        Offset                  _thumb_drag_start_pos;
        Offset                  _thumb_drag_anchor_pos;
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

        void mouse_button(const Position &, int button, Key_state) override;
        void mouse_motion(const Position &) override;
        //void mouse_exit() override;

        void render(Canvas_t *, const Position &offset) override;

    protected:
        using Drag_controller_t = Drag_controller<Config, With_layout>;

        auto scrollbar() { return static_cast<Vertical_scrollbar_t*>(container()); }

        Drag_controller_t       _drag_ctl;
        Offset                  _drag_start_pos;
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

