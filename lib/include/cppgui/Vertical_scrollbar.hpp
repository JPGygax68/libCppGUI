#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "./Container_base.hpp"
#include "./Glyph_button.hpp"
#include "./Resource.hpp"
//#include "./Drag_controller.hpp"

namespace cppgui {

    // Forward declarations

    template <class Impl, class Config, bool With_layout, class Parent> struct Vertical_scrollbar__Layouter;

    // Base class ===================================================

    // Resources struct

    /*
    namespace {

        namespace {
            // TODO: make this adaptive in case color does need translating
            struct slide_bgcol        : Non_mapped_resource<Color> {};
            struct thumb_color        : Non_mapped_resource<Color> {};
            struct thumb_hovered_color: Non_mapped_resource<Color> {};
        }

        template<class Config>
        using Resources = Resource_struct<slide_bgcol, thumb_color, thumb_hovered_color>;
    }
    */

    // Class declaration

    /** TODO: the current implementation works by maintaining a bidirectional, "locked"
            relationship between the position of the thumb and the value it represents.
            This is not necessarily the best approach, since the scrollbar's output value 
            can have a significantly higher resolution than the sliding distance of the
            scrollbar's thumb.
            A better implementation could store the output value as a separate (rational)
            value, and do a one-way synchronization from that value to the thumb's
            position, under the control of the consumer.
     */

    template<class Impl, class Config, bool With_layout>
    class Vertical_scrollbar_base: public 
        Vertical_scrollbar__Layouter<Impl, Config, With_layout, 
            Box<Config, With_layout, 
                Container_base<Config, With_layout, Simple_box_model> > >
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;
        using Container_base_t = typename Container_base<Config, With_layout, Simple_box_model>;
        using Canvas_t = typename Widget_t::Canvas_t;

        using Native_color = typename Widget_t::Native_color;

        using Navigation_handler = std::function<void(Navigation_unit, /* Position initial_pos, */ const Fraction<int> &delta)>; //, bool ending)>;

        Vertical_scrollbar_base();

        /** Defines the values that the scrollbar is going to represent, consisting of 
            the "full" range and the "fraction" range. The former will be represented
            by the "slide" between the the up and down buttons, while the latter is
            reflected by the length of the "thumb".

            Note that this does *not* work as a slider: the highest value that can
            be defined and/or represented by the scrollbar is (full - fraction),
            NOT full.
         */
        void define_values(Length full, Length fraction);

        void init() override;

        void compute_view_from_data() override;

        void mouse_button(const Point &, int button, Key_state, Count clicks) override;
        void mouse_motion(const Point &) override;
        void mouse_wheel(const Vector & ) override;
        void mouse_exit() override;

        void render(Canvas_t *, const Point &offset) override;

        void change_values(Length range, Length thumb);
        auto full_range() const { return _full_range; }
        auto fraction() const { return _fraction; }

        auto current_position() -> Fraction<int>;
        void update_position(Position); // TODO: assert() against calling this while not at end of navigation ?

    protected:
        template<class Parent> using Button_box_model = Fixed_padding_box_model<5, Parent>; // TODO: get constant from somewhere
        using Glyph_button_t = Glyph_button<Config, With_layout, Button_box_model>;
        using Color_resource = typename Widget_t::Color_resource;

        void move_thumb_to(Position);
        void recalc_thumb();
        void clip_thumb_pos();
        void notify_drag_navigation(Position_delta);

        Color_resource          _slide_bgcol;
        Color_resource          _thumb_color;
        Color_resource          _thumb_hovered_color;

        Glyph_button_t          _up_btn, _down_btn;
        Position                _full_range = 0;    // the range represented by the full length of the "slide" (= strip on which the thumb moves)
        Position                _fraction = 0;      // the range represented by the length of the thumb

        Range<Position>         _sliding_range;
        Rectangle               _thumb_rect;

        bool                    _thumb_hovered = false;
        Position                _drag_anchor_pos;
        Position                _curr_drag_pos;
        bool                    _dragging_thumb = false;
    };

    // Layouter aspect

    template <class Impl, class Config, class Parent>
    struct Vertical_scrollbar__Layouter<Impl, Config, true, Parent>: public Parent 
    {
        // Layouter contract

        //void init_layout() override;
        auto get_minimal_size() -> Extents override;
        void layout() override;

        // Extra capabilities coming with layouting
        // TODO

    protected:
        class Vertical_scrollbar_base_t: public Vertical_scrollbar_base<Impl, Config, true> { friend struct Vertical_scrollbar__Layouter; };

        auto p() { return static_cast<Vertical_scrollbar_base_t*>(static_cast<Vertical_scrollbar_base<Impl, Config, true>*>(this)); }
    };

    // Customizable specialization ==================================

    template<class Config, bool With_layout>
    class Custom_vertical_scrollbar:
        public Vertical_scrollbar_base<
            Custom_vertical_scrollbar<Config, With_layout>, Config, With_layout>
    {
    public:
        void on_navigation(Navigation_handler);

    protected:
        friend class Vertical_scrollbar_base<Custom_vertical_scrollbar, Config, With_layout>;

        void move_by_page(int delta);
        void move_by_elements(int delta);
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta);
        void move_by_fraction(const Fraction<int> &delta);

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
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta);
        void move_by_fraction(const Fraction<int> &delta);

    protected:
        void notify_position_change();

        Position_change_handler _on_pos_chng;
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_VERTICAL_SCROLLBAR(Config, With_layout) \
    _CPPGUI_INSTANTIATE_VERTICAL_SCROLLBAR_BASE(Config, With_layout, cppgui::Vertical_scrollbar<Config, With_layout>); \
    template cppgui::Vertical_scrollbar       <Config, With_layout>; \
    template cppgui::Custom_vertical_scrollbar<Config, With_layout>;
