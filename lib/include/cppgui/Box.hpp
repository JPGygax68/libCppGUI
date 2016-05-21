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

#include <array>

#include "./basic_types.hpp"
#include "./layouting.hpp"
#include "./Widget.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Parent> struct Box__Layouter;
    
    // Bordered_box aspect

    template<class Config, bool With_layout, class Parent>
    struct Bordered_box: public Parent
    {
        using Canvas_t = typename Canvas<typename Config::Renderer>;

        void set_border(const Border &border)
        {
            _border = border;
        }
        // TODO: set_border( .. one setting per cardinal direction ...)

        void draw_border(Canvas_t *canvas, const Point &offset)
        {
            auto b = _border.width;

            if (b > 0)
            {
                const auto &rect = p()->rectangle();

                auto ntvclr = Canvas_t::rgba_to_native( _border.color );

                auto x1 = rect.left(), y1 = rect.top(), x2 = rect.right(), y2 = rect.bottom();
                auto w = rect.ext.w, h = rect.ext.h;

                auto d = (Position_delta) b;

                fill_rect(canvas, offset + Point{ x1 + d, y1     }, { w - b,     b }, ntvclr); // top
                fill_rect(canvas, offset + Point{ x2 - d, y1 + d }, {     b, h - b }, ntvclr); // right
                fill_rect(canvas, offset + Point{ x1    , y2 - d }, { w - b,     b }, ntvclr); // bottom
                fill_rect(canvas, offset + Point{ x1    , y1     }, {     b, h - b }, ntvclr); // left
            }
        }

    protected:
        Border      _border {1, {0, 0, 0, 1}}; // TODO: support different borders for each cardinal direction ?

        class Widget_t: public Widget<Config, With_layout> { friend struct Bordered_box; };
        auto p() { return static_cast<Widget_t*>(static_cast<Widget<Config, With_layout>*>(this)); }
    };

    // Bordered_box  Layouter aspect

    template<class Config, class Parent>
    struct Box__Layouter<Config, true, Parent>: public Parent
    {
        void set_padding(Width w)
        {
            set_padding({ w, w, w, w });
        }
        //void set_padding(const std::initializer_list<Length> &);
        void set_padding(const std::array<Width, 4> &padding)
        {
            _padding = padding; // std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
        }

        void add_padding(Rectangle &rect)
        {
            rect.pos.x += _padding[3] - 2;
            rect.pos.y += _padding[0] - 2;
            rect.ext.w -= _padding[3] + _padding[1] - 2 * 2;
            rect.ext.h -= _padding[0] + _padding[2] - 2 * 2;
        }
        auto add_padding(const Rectangle &rect) -> Rectangle
        {
            Rectangle res { rect };

            add_padding( res );

            return res;
        }
        void add_padding(Extents &ext)
        {
            ext.w += _padding[3] + _padding[1];
            ext.h += _padding[0] + _padding[2];
        }
        auto add_padding(const Extents &ext) -> Extents
        {
            Extents res = ext;
            add_padding(res);
            return res;
        }

        Padding     _padding = { 0 };
    };

} // ns cppgui
