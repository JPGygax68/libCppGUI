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

#include "./Widget.hpp"


namespace cppgui
{
    // TODO: make box model a feature of the Widget base class ?

    /*
     *  TODO: move to Textbox.hpp ?
     */
    class Textfield_box_styles
    {
    protected:
        // TODO: make configurable:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 2; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 1, 1, 1, 1 }; }
    };

    class Button_box_styles
    {
    protected:
        // TODO: make configurable:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 4; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA;
    };

    class Container_box_styles
    {
    protected:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 1; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 0.8f, 0.8f, 0.8f, 1}; }
    };

    class Simple_frame_box_styles
    {
    protected:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 0; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 0.8f, 0.8f, 0.8f, 1}; }
    };

    class Text_field_box_styles
    {
    protected:
        static auto border_width    (Widget_states) -> Width { return 0; }
        static auto padding_width   (Widget_states) -> Width { return 3; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 1, 1, 1, 1}; }
    };

    class Borderless_button_box_styles: public Button_box_styles
    {
    protected:
        static auto border_width    (Widget_states) -> Width { return 0; }
    };


    template<class Styles>
    class Box: protected Styles
    {
    public:
        
        void draw_background_and_border(Canvas *c, const Point &p, Bbox_cref b, const Widget_states &s)
        {
            /*
            * We draw two solid rectangles over each other, the first in the border color, then a second,
            * smaller one with the background color.
            * TODO: better approach ? define a ready-made method in Canvas, or even in Renderer ?
            */
            auto r = Rectangle{b};
            c->fill_rect(r + p, this->border_color(s));
            r.inflate(- this->border_width(s), - this->border_width(s));
            c->fill_rect(r + p, this->background_color(s));
        }

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

        auto adjust_box_bounds(Bbox_cref b, int sign = 1) -> Bbox
        {
            return b.expand(sign * (this->border_width({}) + this->padding_width({})));
        }

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };













    #ifdef NOT_DEFINED

    // Base class, providing common functionality

    struct Box_model_base
    {
        static auto box_rectangle() -> Rectangle;

        static auto content_rectangle() -> Rectangle;

        // TODO: this belongs into the Layouter aspect
        auto position_text_element(const Text_bounding_box & bbox, Alignment minor_align, Alignment major_align) const -> Point;

        auto border_color(int /*dir*/)
        {
            // TODO: return value that takes enabled(), hovered(), focused() properties into account
            return RGBA{ 0, 0, 0, 1 }; // TODO: styling!
        }

    protected:
        struct Implementation: Impl { friend struct Aspect; };
        auto p() { return static_cast<Implementation*>(this); }
        auto p() const { return static_cast<const Implementation*>(this); }

        constexpr auto box_inset(int border) const
        {
            return p()->margin(border);
        }

        constexpr auto content_inset(int border) const
        {
            return box_inset(border) + p()->border_width(border) + p()->padding(border);
        }
    };

    // Box_model specialization for build-time definition

    #if defined CPPGUI_BOX_MODEL_BUILD_TIME
    
    template <class Impl>
    struct Box_model
    {
        // All setters are no-ops in the build-time implementation
        static void set_margin      (int dir, Width /*w*/) {}
        static void set_border_width(int dir, Width /*w*/) {}
        static void set_padding     (int dir, Width /*w*/) {}

        auto add_boxing(const Extents & ext) -> Extents;
        void draw_border(Canvas * canvas, const Point & offset);
    };

    #elif defined CPPGUI_BOX_MODEL_RUN_TIME

    // Box_model specialization for run-time definition

    struct Box_model
    {
        // Default values, override in implementations
        static constexpr auto default_margin      (int /*dir*/) { return 0; }
        static constexpr auto default_border_width(int /*dir*/) { return 0; }
        static constexpr auto default_padding     (int /*dir*/) { return 0; }

        Box_model();

        void set_margin      (int /*dir*/, Width w) { _margin = w; }
        void set_border_width(int /*dir*/, Width w) { _border_width = w; }
        void set_padding     (int /*dir*/, Width w) { _padding = w; }

        void set_margin      (Width w) { _margin = w; }
        void set_border_width(Width w) { _border_width = w; }
        void set_padding     (Width w) { _padding = w; }

        auto margin      (int /*dir*/) const { return _margin; }
        auto border_width(int /*dir*/) const { return _border_width; }
        auto padding     (int /*dir*/) const { return _padding; }

        auto add_boxing(const Extents & ext) -> Extents;

        void draw_border(Canvas * canvas, const Point & offset);

    private:
        //class Implementation_t: public Impl { friend struct Aspect; };
        //auto p() { return static_cast<Implementation_t*>(this); }

        Width       _margin = 0;
        Width       _border_width = 1;
        Width       _padding = 2;
    };

    #else
    #error either CPPGUI_BOX_MODEL_RUN_TIME or CPPGUI_BOX_MODEL_BUILD_TIME must be defined
    #endif

    #endif // NOT_DEFINED

} // ns cppgui