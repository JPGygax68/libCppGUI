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
    
    enum class Box_model_definition { build_time, run_time };

    #ifdef NOT_DEFINED

    template<bool PerDirection>
    struct _Box_model_initializer 
    {
        template<class Impl, class Parent>
        struct Aspect {};
    };

    template<>
    struct _Box_model_initializer<false>
    {
        template<class Impl, class Parent>
        struct Aspect
        {
            Aspect()
            {
                p()->set_margin      ( p()->default_margin      () );
                p()->set_border_width( p()->default_border_width() );
                p()->set_padding     ( p()->default_margin      () );

                p()->set_border_color( p()->default_border_color() );
            }

            auto p() { return static_cast<Impl*>(this); }
        };
    };

    template<>
    struct _Box_model_initializer<true>
    {
        template<class Impl, class Parent>
        struct Aspect
        {
            Aspect()
            {
                p()->set_margin      ( 0, p()->default_margin      (0) );
                p()->set_border_width( 0, p()->default_border_width(0) );
                p()->set_padding     ( 0, p()->default_margin      (0) );
                p()->set_border_color( 0, p()->default_border_color(0) );

                p()->set_margin      ( 1, p()->default_margin      (1) );
                p()->set_border_width( 1, p()->default_border_width(1) );
                p()->set_padding     ( 1, p()->default_margin      (1) );
                p()->set_border_color( 1, p()->default_border_color(1) );

                p()->set_margin      ( 2, p()->default_margin      (2) );
                p()->set_border_width( 2, p()->default_border_width(2) );
                p()->set_padding     ( 2, p()->default_margin      (2) );
                p()->set_border_color( 2, p()->default_border_color(2) );

                p()->set_margin      ( 3, p()->default_margin      (3) );
                p()->set_border_width( 3, p()->default_border_width(3) );
                p()->set_padding     ( 3, p()->default_margin      (3) );
                p()->set_border_color( 3, p()->default_border_color(3) );
            }

            auto p() { return static_cast<Impl*>(this); }

            auto p() { return static_cast<Impl*>(this); }
        };
    };

    #endif

    // Base class, providing common functionality

    template<class Config, bool With_layout>
    struct Box_model_base
    {
        template<class Impl, class Parent>
        struct Aspect: Parent
        {
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
            auto position_text_element(const Text_bounding_box &bbox, Alignment minor_align, Alignment major_align) const -> Point
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

            auto border_color(int /*dir*/)
            {
                // TODO: return value that takes enabled(), hovered(), focused() properties into account
                return Color{ 0, 0, 0, 1 }; // TODO: styling!
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
            // All setters are no-ops in the build-time implementation
            static void set_margin      (int dir, Width /*w*/) {}
            static void set_border_width(int dir, Width /*w*/) {}
            static void set_padding     (int dir, Width /*w*/) {}
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
            // Default values, override in implementations
            static constexpr auto default_margin      (int /*dir*/) { return 0; }
            static constexpr auto default_border_width(int /*dir*/) { return 0; }
            static constexpr auto default_padding     (int /*dir*/) { return 0; }

            Aspect()
            {
                // The following will all be no-ops in case of build-time box model definition

                p()->set_margin      ( 0, p()->default_margin      (0) );
                p()->set_border_width( 0, p()->default_border_width(0) );
                p()->set_padding     ( 0, p()->default_margin      (0) );

                p()->set_margin      ( 1, p()->default_margin      (1) );
                p()->set_border_width( 1, p()->default_border_width(1) );
                p()->set_padding     ( 1, p()->default_margin      (1) );

                p()->set_margin      ( 2, p()->default_margin      (2) );
                p()->set_border_width( 2, p()->default_border_width(2) );
                p()->set_padding     ( 2, p()->default_margin      (2) );

                p()->set_margin      ( 3, p()->default_margin      (3) );
                p()->set_border_width( 3, p()->default_border_width(3) );
                p()->set_padding     ( 3, p()->default_margin      (3) );
            }

            void set_margin      (int /*dir*/, Width w) { _margin = w; }
            void set_border_width(int /*dir*/, Width w) { _border_width = w; }
            void set_padding     (int /*dir*/, Width w) { _padding = w; }

            void set_margin      (Width w) { _margin = w; }
            void set_border_width(Width w) { _border_width = w; }
            void set_padding     (Width w) { _padding = w; }

            auto margin      (int /*dir*/) const { return _margin; }
            auto border_width(int /*dir*/) const { return _border_width; }
            auto padding     (int /*dir*/) const { return _padding; }

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

            Width       _margin = 0;
            Width       _border_width = 1;
            Width       _padding = 2;
        };
    };

} // ns cppgui