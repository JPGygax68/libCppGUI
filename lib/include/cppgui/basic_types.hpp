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

#include <cassert>

#include <gpc/gui/renderer.hpp>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/bounding_box.hpp>
#include <gpc/fonts/rasterized_glyph_cbox.hpp>

#include "./geometry.hpp" // TODO: remove, leave choice of whether to include to user code

namespace cppgui {

    using Rasterized_font   = gpc::fonts::rasterized_font;
    using Font_size         = unsigned int;
    using Serialized_font   = std::pair<const uint8_t *, std::size_t>;

    using Text_bounding_box = gpc::fonts::bounding_box;
    using Glyph_control_box = gpc::fonts::rasterized_glyph_cbox;
    using Color             = gpc::gui::rgba_norm;
    using Mono_value        = gpc::gui::mono_norm;

    using Index             = int; // Index is signed so that -1 can be used to report "invalid" or "not found"
    using Count             = unsigned int;

    template<typename T = unsigned int>
    struct Fraction {
        T num, den;

        template<typename Float>
        constexpr auto Float() const -> Float { return static_cast<Float>(static_cast<double>(num) / static_cast<double>(den)); }
       
        constexpr operator T () const { return num / den; }
    };

    template<typename T>
    struct Range
    {
        T from, to;

        auto& operator = (const Range &src) { from = src.from, to = src.to; return *this; }
        auto& operator = (Range &&src) { from = std::move(src.from), to = std::move(src.to); return *this; }

        void define(const T &from_, const T &to_) { from = from_, to = to_; }

        constexpr auto length() const { return to - from; }
    };

    /*
    enum Alignment {
        none = 0,
        left, center, middle = center, right,
        top, bottom
        // TODO: additional values for culture-adaptive settings, or reuse the geometrical ones above ?
    };
    */

    // TODO: use this everywhere!
    // TODO: rename to Simple_border ?
    // TODO: plural form ? (std::array<> for four cardinal directions)

    struct Border {
        Width       width;
        Color       color;
    };

    template<class NativeColorT>
    struct Mapped_border {
        Width           width;
        NativeColorT    color;
    };

    using Separator = Border; // TODO: if Border gets more sophisticated, this will need to become its own structure
    template<class Renderer> using Mapped_separator = Mapped_border<Renderer>;

    enum Navigation_unit {
        full_range                  /* 0: to beginning, 1: to end */, 
        page,
        fraction                    /* when dragging the thumb, expressed as 1/1000000 of the range */,
        element
    };

} // ns cppgui

