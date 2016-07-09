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

#include "./geometry.hpp"

#include "./Widget.hpp"

namespace cppgui
{
    
    #ifndef NOT_DEFINED

    enum class Box_model_definition { build_time, run_time };

    // Base class, providing common functionality

    template<class Config, bool With_layout>
    struct Box_model_base
    {
        template<class Impl, class Parent>
        struct Aspect: Parent
        {
            //constexpr bool has_border () const { return p()->border_width() > 0; };
            //constexpr bool has_padding() const { return p()->padding     () > 0; };

            constexpr auto box_rectangle() const -> Rectangle
            {
                return {
                    {
                        p()->box_inset(3), 
                        p()->box_inset(0)
                    }, 
                    { 
                        this->extents().w - p()->box_inset(3) - p()->box_inset(1), 
                        this->extents().h - p()->box_inset(0) - p()->box_inset(2) 
                    } 
                };
            }

            constexpr auto content_rectangle() const -> Rectangle
            {
                return {
                    {
                        p()->content_inset(3), 
                        p()->content_inset(0)
                    }, 
                    { 
                        this->extents().w - p()->content_inset(3) - p()->content_inset(1), 
                        this->extents().h - p()->content_inset(0) - p()->content_inset(2) 
                    } 
                };
            }

            // TODO: this belongs into the Layouter aspect
            auto position_text_element(const Text_bounding_box &bbox, Alignment minor_align, Alignment major_align) -> Point
            {
                auto r = this->content_rectangle(); // this->extents() );
                Point pos;

                if (minor_align == Alignment::cultural_minor_start)
                {
                    pos.x = r.pos.x;
                }
                else if (minor_align == Alignment::cultural_minor_middle)
                {
                    pos.x = r.left() + (r.ext.w - bbox.width()) / 2;
                }
                else if (minor_align == Alignment::cultural_minor_end)
                {
                    pos.x = r.right() - bbox.width();
                }

                if (major_align == Alignment::cultural_major_start)
                {
                    pos.y = r.top() + bbox.y_max;
                }
                else if (major_align == Alignment::cultural_major_middle)
                {
                    pos.y = r.top() + (r.ext.h - bbox.height()) / 2 + bbox.y_max;
                }
                else if (major_align == Alignment::cultural_major_end)
                {
                    pos.y = r.bottom() + bbox.y_min;
                }
                else
                {
                    assert(false); return {};
                }

                return pos;
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
    };

    // Box model template declaration

    template<class Config, bool With_layout, Box_model_definition BMDef> 
    struct Box_model 
    { 
        template<class Impl, class Parent>
        struct Aspect: Parent {};
    };

    // Box_model specialization for build-time definition

    template<class Config, bool With_layout>
    struct Box_model<Config, With_layout, Box_model_definition::build_time>
    {
        template<class Impl, class Parent>
        struct Aspect: Box_model_base<Config, With_layout>::template Aspect<Impl, Parent>
        {
            // Default values, override in implementation
            static constexpr auto margin      (int border) { return 0; }
            static constexpr auto border_width(int border) { return 0; }
            static constexpr auto padding     (int border) { return 0; }

            auto border_color(int border)
            {
                // TODO: return value that takes enabled(), hovered(), focused() properties into account
                return Color{ 0, 0, 0, 1 };
            }
        };
    };

    // Box_model specialization for run-time definition

    template<class Config, bool With_layout>
    struct Box_model<Config, With_layout, Box_model_definition::run_time>
    {
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        template<class Impl, class Parent>
        struct Aspect: Box_model_base<Config, With_layout>::template Aspect<Impl, Parent>
        {
            auto margin      (int /*border*/) const { return _margin; }
            auto border_width(int /*border*/) const { return _border_width; }
            auto padding     (int /*border*/) const { return _padding; }

            auto border_color(int /*border*/)
            {
                // TODO: adapt to enabled(), hovered(), focused()
                return _border_color;
            }

            auto add_boxing(const Extents &ext) -> Extents
            {
                return { 
                    ext.w + this->content_inset(3) + this->content_inset(1),
                    ext.h + this->content_inset(0) + this->content_inset(2)
                };
            }

            void draw_border(Canvas_t *canvas,  const Point &offset)
            {
                auto rect = this->box_rectangle();
                Width w;

                //bool ena = true; // this->enabled(); // TODO: implement enabled() in Widget<> !!
                //bool hov = this->hovered();
                //bool foc = this->has_focus();

                w = p()->border_width(0);
                this->fill_rect(canvas, { rect.pos + Point{w, 0}, {rect.ext.w - w, w} }, offset, 
                    Canvas_t::rgba_to_native( p()->border_color(0) ) );
                w = p()->border_width(1);
                this->fill_rect(canvas, { rect.pos + Point{rect.ext.w - w, 0}, {w, rect.ext.h} }, offset, 
                    Canvas_t::rgba_to_native( p()->border_color(1) ) );
                w = p()->border_width(2);
                this->fill_rect(canvas, { rect.pos + Point{0, rect.ext.h - w}, {rect.ext.w - w, w} }, offset, 
                    Canvas_t::rgba_to_native( p()->border_color(2) ) );
                w = p()->border_width(3);
                this->fill_rect(canvas, { rect.pos, {w, rect.ext.h} }, offset, 
                    Canvas_t::rgba_to_native( p()->border_color(2) ) );
            }

        private:
            auto p() { return static_cast<Impl*>(this); }

            Length      _margin = 0;
            Length      _border_width = 0;
            Color       _border_color = { 0, 0, 0, 1 };
            Length      _padding = 0;
        };
    };

    #else

    template<class Impl, class Parent>
    struct Box_model: Parent
    {
        static constexpr bool has_border () { return false; };
        static constexpr bool has_padding() { return false; };

        // static constexpr auto inner_rectangle(const Extents &ext) { return ext; }

        constexpr auto inner_rectangle(const Extents &ext) const -> Rectangle
        {
            return { 
                { p()->get_distance(3), p()->get_distance(0) }, 
                { ext.w - p()->get_distance(3) - p()->get_distance(1), ext.h - p()->get_distance(0) - p()->get_distance(2) } 
            };
        }

        static constexpr auto border_rectangle(const Extents &ext) -> Rectangle
        {
            // TODO: margins!
            return { { 0, 0 }, ext };
        }

        constexpr auto get_margin(int /*border*/) const { return 0; }

        constexpr auto get_border_width(int /*border*/) const { return 0; }
        
        // TODO: indirect via protected method (does not need default values for parameters) ?
        constexpr auto get_border_color(int border, bool enabled = true, bool hovered = false, bool focused = false) const { return Color{ 0, 0, 0, 1}; }
        
        constexpr auto get_padding(int /*border*/) const { return 0; }
        
        constexpr auto get_distance(int border) const { return p()->get_margin(border) + p()->get_border_width(border) + p()->get_padding(border); }

    private:
        auto p() { return static_cast<Impl*>(this); }
        auto p() const { return static_cast<const Impl*>(this); }
    };
    
    template<class Parent>
    struct Simple_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        //static constexpr auto inner_rectangle(const Extents &ext) { return { Extents{ 0, 0 }, ext }; }
        //static constexpr auto get_border_width(int border) { return 0; }
        //static constexpr auto get_padding(int border) { return 0; }
    };

    /** This box model has no border, no margin and only a fixed-width padding.
     */
    template<Width Width, class Parent>
    struct Fixed_padding_box_model: Box_model<Fixed_padding_box_model<Width, Parent>, Parent>
    {
        static constexpr bool has_padding() { return true; }

        static constexpr auto get_padding(int /*border*/) { return Width; }
    };

    template<Width BorderWidth, Width PaddingWidth, class Parent>
    struct Fixed_border_and_padding_box_model: 
        Box_model<Fixed_border_and_padding_box_model<BorderWidth, PaddingWidth, Parent>, 
            Parent>
    {
        static constexpr bool has_border () { return true; }
        static constexpr bool has_padding() { return true; }

        static constexpr auto get_border_width(int /*border*/) { return BorderWidth; }

        auto get_border_color(int /*border*/, bool /*enabled*/ = true, bool hovered = false, bool /*focused*/ = false) const
        {
            if (hovered)
                return Color{ 0, 0, 0.5f, 1};
            else
                return Color{ 0, 0, 0, 1};
        }

        static constexpr auto get_padding(int /*border*/) { return PaddingWidth; }
    };

    // Wrappers that give access to standardized templates (taking Parent for parameter = "aspects")

    template<Width BorderWidth, Width PaddingWidth>
    struct Fixed_border_and_padding
    {
        template<class Parent> using Box_model = Fixed_border_and_padding_box_model<BorderWidth, PaddingWidth, Parent>;
    };

    #endif

} // ns cppgui