#pragma once

#include "./Box.hpp"

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

#include "./Canvas.ipp"
#include "./Widget.ipp"

namespace cppgui {

    #ifdef NOT_DEFINED

    template<class Config, bool With_layout, class Parent>
    void Bordered_box<Config, With_layout, Parent>::set_border(const Border &border)
    {
        _border = border;
    }

    template<class Config, bool With_layout, class Parent>
    void Bordered_box<Config, With_layout, Parent>::draw_border(Canvas_t *canvas, const Rectangle &rect, const Point & offset)
    {
        auto b = _border.width;

        if (b > 0)
        {
            auto ntvclr = Canvas_t::rgba_to_native( _border.color );

            auto x1 = rect.left(), y1 = rect.top(), x2 = rect.right(), y2 = rect.bottom();
            auto w = rect.ext.w, h = rect.ext.h;

            auto d = b;

            canvas->fill_rect(offset.x + x1 + d, offset.y + y1    ,  w - b,     b, ntvclr);
            canvas->fill_rect(offset.x + x2 - d, offset.y + y1 + d,      b, h - b, ntvclr);
            canvas->fill_rect(offset.x + x1    , offset.y + y2 - d,  w - b,     b, ntvclr);
            canvas->fill_rect(offset.x + x1    , offset.y + y1    ,      b, h - b, ntvclr);

            /*
            fill_rect(canvas, offset + Point{ x1 + d, y1     }, { w - b,     b }, ntvclr); // top
            fill_rect(canvas, offset + Point{ x2 - d, y1 + d }, {     b, h - b }, ntvclr); // right
            fill_rect(canvas, offset + Point{ x1    , y2 - d }, { w - b,     b }, ntvclr); // bottom
            fill_rect(canvas, offset + Point{ x1    , y1     }, {     b, h - b }, ntvclr); // left
            */
        }
    }

    /*
    template<class Config, bool With_layout, class Parent>
    void Bordered_box<Config, With_layout, Parent>::draw_vert_separator(Canvas_t * canvas, const Point & offset, const Separator & sep, Position at)
    {
        fill_rect(canvas, { at, _border.width, sep.width, this->extents().h - 2 * _border.width }, 
            this->position() + offset, Canvas_t::rgba_to_native(sep.color) );
    }
    */

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    void Box__Layouter<Config, true, Parent>::set_padding(Width w)
    {
        set_padding({ w, w, w, w });
    }

    template<class Config, class Parent>
    void Box__Layouter<Config, true, Parent>::set_padding(const std::array<Length, 4> &padding)
    {
        _padding = padding; // std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Config, class Parent>
    void Box__Layouter<Config, true, Parent>::add_padding(Rectangle &rect)
    {
        rect.pos.x += _padding[3] - 2;
        rect.pos.y += _padding[0] - 2;
        rect.ext.w -= _padding[3] + _padding[1] - 2 * 2;
        rect.ext.h -= _padding[0] + _padding[2] - 2 * 2;
    }

    template<class Config, class Parent>
    auto Box__Layouter<Config, true, Parent>::add_padding(const Rectangle &rect) -> Rectangle
    {
        Rectangle res { rect };

        add_padding( res );

        return res;
    }

    template<class Config, class Parent>
    auto Box__Layouter<Config, true, Parent>::add_padding(const Extents &ext) -> Extents
    {
        Extents res = ext;
        
        res.w += _padding[3] + _padding[1];
        res.h += _padding[0] + _padding[2];

        return res;
    }

    template<class Config, class Parent>
    void Box__Layouter<Config, true, Parent>::add_padding(Extents &ext)
    {
        ext.w += _padding[3] + _padding[1];
        ext.h += _padding[0] + _padding[2];
    }

    #endif

    template <class Config>
    template <class Parent>
    auto _box<Config>::Layouter<true, Parent>::position_text_element(const Text_bounding_box & bbox, Alignment minor_align, Alignment major_align) -> Point
    {
        auto r = this->inner_rectangle( this->extents() );
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

        /*
        // Rectangle around text
        *rectangle = {
            pos.x + bbox.x_min, pos.y - bbox.y_max,
            static_cast<Length>(bbox.width()), static_cast<Length>(bbox.height())
        };
        */
    }

} // ns cppgui