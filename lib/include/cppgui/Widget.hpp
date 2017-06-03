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

#include <functional>
#include "./UI_element.hpp"
#include "./layouting.hpp"


namespace cppgui {

    // Forward / undefined declarations
    class Container_base;
    class Internal_popup;

    // Widget 

    class Widget: public UI_element
    {
    public:
        using Click_handler     = std::function<bool(const Point &, int button, Count clicks)>; // TODO: support return value ?
        using Pushed_handler    = std::function<bool()>; // for buttons TODO: renamed event to "Push" (as in "a push happened") ?

        Widget();
        virtual ~Widget() {}

        void set_id(const char *id)
        {
            #ifdef _DEBUG
            _id = id;
            #endif
        }

        void on_click(Click_handler);

        /** The compute_view_from_data() entry point must be called after init(), and also after 
            layout() if run-time layouting is enabled.
        */
        virtual void compute_view_from_data() {}

        void added_to_container(Container_base *);
        void removed_from_container(Container_base *);

        // State queries

        bool disabled() const { return false; } // TODO!!!

        bool is_first_child();
        bool is_last_child();

        // Keyboard focus

        virtual bool focussable() const { return visible(); }
        virtual void take_focus();
        virtual void gained_focus();
            // Important: do not call gained_focus() from a child; call child_obtained_focus() instead,
            // to inform the container.
        virtual void loosing_focus();
        virtual bool has_focus();

        // Input event handling

        /*
            By convention, mouse positions are passed to a widget as relative to
            their own origin (meaning that it falls to the caller, i.e. usually
            the container, to subtract the child widget's position() from the
            coordinates it gets from yet higher up).

            TODO: event injection methods should report whether or not the event
                was consumed.
         */
        bool mouse_motion(const Point &) override;
        bool mouse_button(const Point &, int /*button*/, Key_state, Count clicks) override;
        bool mouse_click(const Point &, int button, Count count) override;
        bool mouse_wheel(const Vector &) override;
        bool text_input(const char32_t *, size_t) override;
        bool key_down(const Keycode &) override;
        //bool key_up(const Keycode &) override;

        // Queries

        auto absolute_position() -> Point override;

        // Run-time manipulations

        void change_visible(bool visible = true);

        // Misc 

        auto container() const -> Container_base * { return _container; }
        virtual auto root_widget() -> Root_widget*;

    protected:

        
        void pass_up_and_notify_focus();

        void invalidate() override;

        // Rendering conveniences

        // auto rgba_to_native(Canvas *, const RGBA &) -> Native_color;
        //auto rgba_to_native(const RGBA &) -> Native_color;
        //void fill_rect(Canvas *, const Point &pos, const Extents &ext, const Native_color &);
        //void fill(Canvas *, const Point &offs, const Native_color &);
        //auto convert_position_to_inner(const Point &) -> Point;
        static auto advance_to_glyph_at(const Rasterized_font *, const std::u32string &text, size_t from, size_t to, Point &pos) 
            -> const Glyph_control_box *;

        // Visual state
        virtual auto visual_states() -> Widget_states;

        // Experimental & temporary: implement more sophisticated (and flexible!) styling
        // - May not / should not stay static; make const if possible

        static auto stroke_width() -> int { return 1; }
        static auto stroke_color() -> RGBA { return { 0, 0, 0, 1 }; }
        //static auto padding() -> int { return 5; }
        static auto paper_color() -> RGBA { return {1, 1, 1, 1}; }

        // Static styles
        // TODO: move to "stylesheet"
        static constexpr auto default_dialog_background_color() -> RGBA      { return {0.6f, 0.6f, 0.6f, 1}; }
        static constexpr auto widget_background_color        () -> RGBA      { return { 1, 1, 1, 1 }; }
        static constexpr auto interior_separator             () -> Separator { return { 1, { 0.2f, 0.2f, 0.2f, 1 } }; }
        static constexpr auto grid_separator                 () -> Separator { return { 1, { 0.4f, 0.4f, 0.4f, 1 } }; }
        static constexpr auto item_background_color          () -> RGBA      { return { 0.7f, 0.7f, 0.7f, 1 }; }
        static constexpr auto selected_item_background_color () -> RGBA      { return { 0.9f, 0.9f, 0.9f, 1 }; }
        static constexpr auto hovered_item_background_color  () -> RGBA      { return { 0.8f, 0.8f, 0.8f, 1 }; }

        // Styling
        // TODO: replace with actual styling/themes
        auto button_face_color  () -> RGBA;
        auto button_border_color() -> RGBA;
        auto button_border_width() -> int;

        Container_base             *_container = nullptr;

    private:

        #ifdef _DEBUG
        const char  *_id;
        #endif

        Click_handler           _click_hndlr;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        void set_bounds(const Pbbox &lb, Alignment h_align = horigin, Alignment v_align = baseline);
        void set_bounds(const Point &, Bbox_cref, Alignment h_align = horigin, Alignment v_align = baseline);
        void set_rectangle(const Point &, const Extents &, Alignment h_ref=left, Alignment v_ref=top);

        // To be provided by concrete classes
        virtual void init_layout() = 0;
        virtual auto get_minimal_bbox() -> Bbox = 0;
        virtual void compute_layout(Bbox_cref) {}

        // Overridable for specialization
        virtual auto get_optimal_bounds() -> Bbox { return get_minimal_bbox(); }

    protected:
        // TODO: this really belongs to a utility module
        static auto align_cultural_minor(Bbox_cref inner, Bbox_cref outer, Alignment align) -> Position_delta;
        
        //void set_padding(Width);
        //void set_padding(const std::initializer_list<Width> &);

        //void set_rectangle(const Point &nw, const Point &se);
        //void set_rectangle_nw(const Point &, const Extents &);
        //void set_rectangle_ne(const Point &, const Extents &);
        //void set_rectangle_se(const Point &, const Extents &);
        //void set_rectangle_sw(const Point &, const Extents &);

    #endif // CPPGUI_EXCLUDE_LAYOUTING

    }; // class Widget


    // Decorators

    template<class WidgetC>
    class Non_focussable: public WidgetC
    {
    public:
        bool focussable() const override { return false; }
    };

} // ns cppgui
