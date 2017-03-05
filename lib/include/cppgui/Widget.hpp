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

#include <functional>

#include <cppgui_config.hpp>

#include "./basic_types.hpp"
#include "./Canvas.hpp"
#include "./Resource.hpp"

#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER


namespace cppgui {

    enum Key_state { pressed, released }; // TODO: move to basic_types.hpp ?

    class Root_widget;
    class Container;

    // Widget 

    class Widget
    {
    public:
        using Click_handler     = std::function<void(const Point &, int button, Count clicks)>; // TODO: support return value ?
        using Pushed_handler    = std::function<void()>; // for buttons TODO: renamed event to "Push" (as in "a push happened") ?

        Widget();

        void set_id(const char *id)
        {
            #ifdef _DEBUG
            _id = id;
            #endif
        }

        //auto& rectangle() { return _rect; }
        //auto& rectangle() const { return _rect; }
        auto& position() { return _position; }
        auto& position() const { return _position; }
        //auto& extents() { return _rect.ext; }
        //auto& extents() const { return _rect.ext; }
        auto& bounds() { return _bounds; }
        auto& bounds() const { return _bounds; }
        //void set_position(const Point &);
        //void set_bounds(const Bounding_box &);

        void set_background_color(const RGBA &);
        auto background_color() const -> RGBA;

        void on_click(Click_handler);

        /** The init() entry point is where a widget "connects" to its backends (the most important of
            which being the canvas).
        */
        virtual void init() {}

        /** The compute_view_from_data() entry point must be called after init(), and also after 
            layout() if run-time layouting is enabled.
        */
        virtual void compute_view_from_data() {}

        // Run-time properties

        void set_visible(bool visible = true);
        bool visible() const { return _visible; }

        bool disabled() const { return false; } // TODO!!!

        void set_focussable(bool state = true) { _focussable = state; }
        bool focussable() const { return _focussable; } // TODO: replace with can_take_focus() that takes other factors into consideration ?

        void added_to_container(Container *);
        void removed_from_container(Container *);

        // TODO: should the following be protected ?
        bool hovered() const { return _hovered; }

        virtual void take_focus();
        /** Important: do not call gained_focus() from a child; call child_obtained_focus() instead,
            to inform the container.
         */
        virtual void gained_focus();
        virtual void loosing_focus();
        virtual bool has_focus();

        bool is_first_child();
        bool is_last_child();

        // Input event injection

        /** By convention, mouse positions are passed to a widget as relative to
            their own origin (meaning that it falls to the caller, i.e. usually
            the container, to subtract the child widget's position() from the
            coordinates it gets from yet higher up).
         */
        virtual void mouse_motion(const Point &) {}
        virtual void mouse_button(const Point &, int /*button*/, Key_state, Count clicks);
        virtual void mouse_click(const Point &, int button, Count count);
        virtual void mouse_wheel(const Vector &) {}
        virtual void text_input(const char32_t *, size_t) {}
        virtual void key_down(const Keycode &);
        //void key_up(const Keycode &);

        virtual void mouse_enter();
        virtual void mouse_exit ();

        // Queries

        virtual auto absolute_position() -> Point;
        bool contains_point(const Point &point);

        // Run-time manipulations

        void change_visible(bool visible = true);

        // Rendering

        virtual void render(Canvas *, const Point &offset) = 0;

        // Event handling chains

        virtual bool handle_key_down(const Keycode &) { return false; }

        // Misc 

        auto container() const -> Container * { return _container; }

    protected:

        virtual auto root_widget() -> Root_widget*;

        void pass_up_and_notify_focus();

        // Graphics system integration
        void shift_horizontally(Position_delta);
        void shift_vertically(Position_delta);
        void shift_up  (Length);
        void shift_down(Length);

        // Rendering conveniences

        // auto rgba_to_native(Canvas *, const RGBA &) -> Native_color;
        auto rgba_to_native(const RGBA &) -> Native_color;
        void fill_rect(Canvas *, const Rectangle &rect, const Native_color &);
        void fill_rect(Canvas *, const Rectangle &rect, const Point &offs, const Native_color &);
        void fill_rect(Canvas *, const Point &pos, const Extents &ext, const Native_color &);
        void fill(Canvas *, const Point &offs, const Native_color &);
        auto convert_position_to_inner(const Point &) -> Point;
        auto advance_to_glyph_at(const Rasterized_font *, const std::u32string &text, size_t from, size_t to, Point &pos) 
            -> const Glyph_control_box *;
        void draw_borders(Canvas *, const Point & offs, Width width, const RGBA &color);
        void draw_borders(Canvas *, const Rectangle &rect, const Point &offs, Width width, const RGBA &color);
        void draw_borders(Canvas *, const Rectangle &rect, const Point &offs, 
            Width width, const RGBA & top, const RGBA & right, const RGBA & bottom, const RGBA & left);
        // PROVISIONAL
        //void draw_stippled_inner_rect(Canvas *, const Rectangle &, const Point &offs);

        // Experimental & temporary: implement more sophisticated (and flexible!) styling
        // - May not / should not stay static; make const if possible

        static auto stroke_width() -> int { return 1; }
        static auto stroke_color() -> RGBA { return { 0, 0, 0, 1 }; }
        //static auto padding() -> int { return 5; }
        static auto paper_color() -> RGBA { return {1, 1, 1, 1}; }

        // Static styles
        // TODO: move to "stylesheet"
        static constexpr auto default_dialog_background_color() -> RGBA     { return {0.6f, 0.6f, 0.6f, 1}; }
        static constexpr auto widget_background_color        () -> RGBA     { return { 1, 1, 1, 1 }; }
        static constexpr auto interior_separator             () -> Separator { return { 1, { 0.2f, 0.2f, 0.2f, 1 } }; }
        static constexpr auto grid_separator                 () -> Separator { return { 1, { 0.4f, 0.4f, 0.4f, 1 } }; }
        static constexpr auto item_background_color          () -> RGBA     { return { 0.7f, 0.7f, 0.7f, 1 }; }
        static constexpr auto selected_item_background_color () -> RGBA     { return { 0.9f, 0.9f, 0.9f, 1 }; }
        static constexpr auto hovered_item_background_color  () -> RGBA     { return { 0.8f, 0.8f, 0.8f, 1 }; }

        // Styling
        // TODO: move to new class Abstract_button<> ?
        auto button_face_color() -> RGBA;
        auto button_border_color() -> RGBA;
        auto button_border_width() -> int;

        Container                  *_container = nullptr;

        //Rectangle               _inner_rect;

    private:
        friend class Drag_controller;

        #ifdef _DEBUG
        const char  *_id;
        #endif

        Point                   _position = {};
        Bounding_box            _bounds = {};
        RGBA                    _bkgnd_clr = {0, 0, 0, 0};
        Click_handler           _click_hndlr;
        bool                    _visible = true;
        bool                    _focussable = true;
        bool                    _hovered = false;

    public:
        //-----------------------------------------------------------
        // "Updater" "aspect
        // TODO: make configurable by preprocessor

        void invalidate();

        // END of Updater aspect
        //-----------------------------------------------------------

        // Layouting aspect -----------------------------------------
        // TODO: make optional via preprocessor

    public:
        virtual void init_layout() = 0;
        virtual auto get_minimal_bounds() -> Bounding_box = 0;
        //virtual auto get_preferred_size() -> Extents { return get_minimal_size(); }
        virtual void set_bounds(const Point & p, const Bounding_box & b) = 0;

        //void set_padding(Width);
        //void set_padding(const std::initializer_list<Width> &);

        //void set_rectangle(const Point &nw, const Point &se);
        //void set_rectangle_nw(const Point &, const Extents &);
        //void set_rectangle_ne(const Point &, const Extents &);
        //void set_rectangle_se(const Point &, const Extents &);
        //void set_rectangle_sw(const Point &, const Extents &);

        // END of Layouting aspect ----------------------------------

    }; // class Widget

} // ns cppgui
