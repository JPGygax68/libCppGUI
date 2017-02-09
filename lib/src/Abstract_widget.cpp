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

#include <iostream> // TODO: REMOVE!!

#include <cppgui/Widget.hpp>
#include <cppgui/Container_base.hpp>
#include <cppgui/Root_widget.hpp>

namespace cppgui {

    // Abstract_widget<> --------------------------------------------

    void Abstract_widget::set_position(const Point &pos)
    {
        _rect.pos = pos;
    }

    void Abstract_widget::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    /*
    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::mouse_click(const Point &, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            // TODO
        }
    }
    */

    auto Abstract_widget::rgba_to_native(const Color &color) -> Native_color
    {
        return Canvas::rgba_to_native( color );
    }

    void Abstract_widget::fill_rect(Canvas *r, const Rectangle &rect, const Native_color &color)
    {
        r->fill_rect(rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h, color);
    }

    void Abstract_widget::fill_rect(Canvas *r, const Rectangle & rect, const Point & offs, const Native_color &color)
    {
        r->fill_rect(rect.pos.x + offs.x, rect.pos.y + offs.y, rect.ext.w, rect.ext.h, color);
    }

    void Abstract_widget::fill_rect(Canvas *r, const Point & pos, const Extents & ext, const Native_color &color)
    {
        r->fill_rect(pos.x, pos.y, ext.w, ext.h, color);
    }

    void Abstract_widget::fill(Canvas *r, const Point &offs, const Native_color &color)
    {
        fill_rect(r, rectangle(), offs, color);

        //auto b{ rectangle() };
        //r->fill_rect(offs.x + b.pos.x, offs.y + b.pos.y, b.ext.w, b.ext.h, color);
    }

    void Abstract_widget::draw_borders(Canvas *cv, const Point & offs, Width width, const Color &color)
    {
        draw_borders(cv, rectangle(), offs, width, color);
    }

    void Abstract_widget::draw_borders(Canvas *cv, const Rectangle &rect, const Point &offs, 
        Width width, const Color &color)
    {
        draw_borders(cv, rect, offs, width, color, color, color, color);
    }

    void Abstract_widget::draw_borders(Canvas *r, const Rectangle & rect, const Point & offs, 
        Width width, const Color & top, const Color & right, const Color & bottom, const Color & left)
    {
        // TODO: this painting procedure does a "wrap" in clockwise fashion, without regard for corners
        fill_rect(r, offs + rect.pos + Point{width, 0}, {rect.ext.w - width, width}, top);
        fill_rect(r, offs + rect.pos + Point{rect.ext.w - width, 0}, {width, rect.ext.h}, right);
        fill_rect(r, offs + rect.pos + Point{0, rect.ext.h - width}, {rect.ext.w - width, width}, bottom);
        fill_rect(r, offs + rect.pos, {width, rect.ext.h}, left);
    }

    /*
    template <class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_stippled_inner_rect(Canvas_t *canvas, const Rectangle &rect, const Point & offs)
    {
        auto r { rect.pos, Extents{ rect.ext.w - 1, rect.ext.h - 1} };
    }
    */

    auto Abstract_widget::convert_position_to_inner(const Point &pos) -> Point
    {
        // TODO: subtract border and padding
        return pos;
    }

    auto Abstract_widget::advance_to_glyph_at(const Rasterized_font *font, const std::u32string &text, 
        size_t from, size_t to, Point &pos) -> const Glyph_control_box *
    {
        assert(to >= from);

        const Glyph_control_box *cbox = nullptr;

        for (auto i = from; ; i++)
        {
            cbox = & font->lookup_glyph(0, text[i])->cbox;
            if (i == to) break;
            pos.x += cbox->adv_x;
            pos.y += cbox->adv_y;
        }

        return cbox;
    }

    // Display Updater "aspect" -------------------------------------

    void Abstract_widget::invalidate()
    {
        container()->child_invalidated(static_cast<Widget_t*>(this));
    }

} // ns cppgui