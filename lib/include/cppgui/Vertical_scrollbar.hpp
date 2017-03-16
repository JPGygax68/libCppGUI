#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016, 2017 Hans-Peter Gygax

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

#include "./Container.hpp"
#include "./Glyph_button.hpp"
#include "./Boxed.hpp"


namespace cppgui {

    /** Synopsis

        The Scrollbar is a widget that is intended for positioning content that is (potentially) too big to be displayed all at once.
        
        A scrollbar takes two "size" parameters: the "full" size, and the "shown" size. Those parameters are of type Length and must be initially set, together, using the `define_sizes()` method (the values can later be changed using the `change_sizes()` method). They represent, respectively, the full size of the display space (in whatever unit that is defined)

        There is no automatic link between the scrollbar and the view that it is intended to control. Instead, the scrollbar generates "navigation" events, and also accepts position updates to reflect navigation that did not originate from the scrollbar.

        Note that Scrollbar has two specializations: "custom" and "standalone". The Custom variant delegates navigation to an externally provided handler, while Standalone computes the output position value itself and signals *that* to a handler.

        The Custom variant should be used when the "space" being navigated is not strictly linear, or direct control over positioning is desired for some other reason (this includes a possible lack of precision: it is quite possible that the pixel resolution of the scrollbar track is not sufficient to precisely navigate to items contained in the controlled display space).

        Note that users of the Custom variant should/must always call the method `update_thumb_position()` at the end of the navigation to ensure that the thumb's position adequately reflects the new position of the display (however that is defined).

        Scrollbar generates three types of navigation events:
        - "item":       move forward or backward by N items
        - "page":       move forward or backward by N pages [Note: currently N is always = 1 for page navigation]
        - "fraction":   move forward or backward by a fractional amount (this occurs when the user drags the thumb)
     */ 

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
            can have a significantly higher resolution than the number of pixels that
            the thumb can move within the track.
            A better implementation could store the output value as a separate (rational)
            value, and do a one-way synchronization from that value to the thumb's
            position, under the control of the consumer.
     */

    class Vertical_scrollbar_base: public Container_base
    {
    public:

        using Navigation_handler = std::function<void(Navigation_unit, /* Position initial_pos, */ const Fraction<int> &delta)>; //, bool ending)>;

        Vertical_scrollbar_base();

        void define_sizes(Length full, Length shown);

        void init(Canvas *) override;

        void compute_view_from_data() override;

        bool mouse_button(const Point &, int button, Key_state, Count clicks) override;
        bool mouse_motion(const Point &) override;
        bool mouse_wheel(const Vector & ) override;
        void mouse_exit() override;

        void render(Canvas *, const Point &offset) override;

        void change_sizes(Length full, Length shown);
        auto full_range() const { return _full_range; }
            // (Integer) range represented by the length of the sliding zone.
        auto shown_range() const { return _shown_range; }
            // (Integer) range represented by the length of the thumb.

        auto current_value_from_thumb_position() -> Fraction<Length>;
            // Converts the current thumb position to a rational number.
            /*  TODO: this approach may (needlessly ?) sacrifice precision; it may be better to 
                store and update this rational number instead of deriving it from a pixel position. */

        void update_thumb_position(Length value); // TODO: assert() against calling this while not at end of navigation ?
            // Recomputes and updates the thumb position according to the specified value.

    protected:
        using Super = Container_base;

        // Specialization hooks

        virtual void move_by_page(int delta) = 0;
        virtual void move_by_elements(int delta) = 0;
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta) = 0;
        virtual void move_by_fraction(const Fraction<int> &delta) = 0;

        //void move_thumb_to(Position);
        void recalc_thumb();
        void clip_thumb_pos();
        void notify_drag_navigation(Position_delta);

    private:
        RGBA                    _slide_bgcol;           // TODO: use styling
        RGBA                    _thumb_color;           // ditto
        RGBA                    _thumb_hovered_color;   // ditto

        Embedded_glyph_button _up_btn, _down_btn;
        Length                  _full_range = 0;    // the range represented by the full length of the track 
                                                    // (= strip on which the thumb moves)
        Length                  _shown_range = 0;   // the range represented by the length of the thumb

        Range<Position>         _track;             // starting and ending position of the track (as pixel positions)
        Rectangle               _thumb_rect;        // TODO: replace with bounding box ?

        bool                    _thumb_hovered = false;
        Position                _drag_anchor_pos;
        Position                _curr_drag_pos;
        bool                    _dragging_thumb = false;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        //void init_layout() override;
        auto get_minimal_bounds() -> Bbox override;
        void compute_layout(Bbox_cref) override;

        // Extra capabilities coming with layouting
        // TODO

    #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

    // Customizable specialization ==================================

    /* Use this specialization when you need to control navigation directly, with the scrollbar widget acting as both input (arrows, thumb dragging, keyboard) and output (thumb position), but not as the direct controller of the interaction.
     */
    class Custom_vertical_scrollbar: public Vertical_scrollbar_base
    {
    public:
        void on_navigation(Navigation_handler);

    protected:
        friend class Vertical_scrollbar_base;

        void move_by_page(int delta) override;
        void move_by_elements(int delta) override;
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta) override;
        void move_by_fraction(const Fraction<int> &delta) override;

        Navigation_handler      _nav_handler;
    };

    // Standalone specialization ===================================

    /* Use this specialization when there is direct and linear correspondance between the thumb's position inside the track and the position of the target viewable area inside the viewing window (however those are defined).
     */

    class Vertical_scrollbar: public Vertical_scrollbar_base
    {
    public:
        using Position_change_handler = std::function<void(Position)>;

        void on_position_change(Position_change_handler);

        void move_by_page(int delta) override;
        void move_by_elements(int delta) override;
        //void move_by_fraction(Position initial_pos, const Fraction<int> &delta) override;
        void move_by_fraction(const Fraction<int> &delta) override;

    protected:
        void notify_position_change();

        Position_change_handler _on_pos_chng;
    };


    // "Boxed" version --------------------------------------------------------

    class Scrollbar_box_styles // TODO: move to header common to vertical and horizontal
    {
    protected:
        // TODO: make configurable:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 0; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 0.9f, 0.9f, 0.9f, 1}; }
    };

    using Boxed_vertical_scrollbar = Boxed<Vertical_scrollbar, Scrollbar_box_styles>;

    // Other variants ---------------------------------------------------------

    using Embedded_vertical_scrollbar = Non_focussable<Custom_vertical_scrollbar>;

} // ns cppgui
