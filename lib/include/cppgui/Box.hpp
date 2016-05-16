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

        void set_border(const Border &);
        // TODO: set_border( .. one setting per cardinal direction ...)

        void draw_border(Canvas_t *, const Point &offset);

    protected:
        Border      _border {1, {0, 0, 0, 1}}; // TODO: support different borders for each cardinal direction ?

        class Widget_t: public Widget<Config, With_layout> { friend struct Bordered_box; };
        auto p() { return static_cast<Widget_t*>(static_cast<Widget<Config, With_layout>*>(this)); }
    };

    // Bordered_box  Layouter aspect

    template<class Config, class Parent>
    struct Box__Layouter<Config, true, Parent>: public Parent
    {
        void set_padding(Width);
        //void set_padding(const std::initializer_list<Length> &);
        void set_padding(const std::array<Width, 4> &);

        void add_padding(Rectangle &);
        auto add_padding(const Rectangle &) -> Rectangle;
        void add_padding(Extents &);
        auto add_padding(const Extents   &) -> Extents;

        Padding     _padding = { 0 };
    };

} // ns cppgui
