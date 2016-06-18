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

#include "./Canvas.hpp"
#include "./Box_model.hpp"
#include "./Widget.hpp"

namespace cppgui {

    // Forward declaration

    template<class Config, bool With_layout, class Parent> struct Box;

    // Internal templatized namespace

    template<class Config>
    struct _box
    {
        template<bool With_layout, class Parent> struct Layouter;

        using Canvas_t = Canvas<typename Config::Renderer>;

        // Border_drawer --------------------------------------------

        /*  Border_drawer is an aspect that needs to be given a Widget<> descendent for a parent.
            It also uses CRTP.
         */

        template<class Class, class Parent, bool HasBorder = Parent::has_border()> 
        struct Border_drawer: public Parent
        {
            // TODO: obtain rectangle on its own instead
            static void draw_border(Canvas_t *, const Point &offset) {} // default implementation does nothing
        };

        template<class Class, class Parent> 
        struct Border_drawer<Class, Parent, true>: Parent
        {
            void draw_border(Canvas_t *canvas,  const Point &offset)
            {
                auto rect = p()->get_border_rectangle();
                Width w;

                bool ena = true; // this->enabled(); // TODO: implement enabled() in Widget<> !!
                bool hov = this->hovered();
                bool foc = this->has_focus();

                w = p()->get_border_width(0);
                this->fill_rect(canvas, { rect.pos + Point{w, 0}, {rect.ext.w - w, w} }, offset, 
                    Canvas_t::rgba_to_native( p()->get_border_color(0, ena, hov, foc) ) );
                w = p()->get_border_width(1);
                this->fill_rect(canvas, { rect.pos + Point{rect.ext.w - w, 0}, {w, rect.ext.h} }, offset, 
                    Canvas_t::rgba_to_native( p()->get_border_color(1, ena, hov, foc) ) );
                w = p()->get_border_width(2);
                this->fill_rect(canvas, { rect.pos + Point{0, rect.ext.h - w}, {rect.ext.w - w, w} }, offset, 
                    Canvas_t::rgba_to_native( p()->get_border_color(2, ena, hov, foc) ) );
                w = p()->get_border_width(3);
                this->fill_rect(canvas, { rect.pos, {w, rect.ext.h} }, offset, 
                    Canvas_t::rgba_to_native( p()->get_border_color(2, ena, hov, foc) ) );
            }

            auto p() { return static_cast<Class*>(this); }
        };

        // Layouter aspect ------------------------------------------

        // Dummy implementation (without layouting capabilities)
        template<class Parent> struct Layouter<false, Parent>: Parent {};

        // Real implementation

        template<class Parent>
        struct Layouter<true, Parent>: Parent
        {
            auto add_boxing(const Extents &ext) -> Extents
            {
                return { 
                    ext.w + this->get_distance(3) + this->get_distance(1),
                    ext.h + this->get_distance(0) + this->get_distance(2)
                };
            }

            auto position_text_element(const Text_bounding_box &bbox, Alignment minor_align, Alignment major_align) -> Point;
        };

    };

    /** This is an aspect that must be given a parent that IS_A BoxModel implementation and
        IS_A Widget<>.
        TODO: concept checking ?
     */
    template<class Config, bool With_layout, class Parent>
    struct Box: 
        _box<Config>::template Layouter< With_layout,
            _box<Config>::template Border_drawer<
                Box<Config, With_layout, Parent >, 
                Parent > > 
    {
        auto get_border_rectangle() const -> Rectangle { return this->border_rectangle( this->extents() ); }
        auto get_inner_rectangle() const -> Rectangle { return this->inner_rectangle( this->extents() ); }

    };

    // template<class Config, class With_layout, class Parent> using Box__Layouter = typename _box<Config>::template Layouter<With_layout, Parent>;

} // ns cppgui

