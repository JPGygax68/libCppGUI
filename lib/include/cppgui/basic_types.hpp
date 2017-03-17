#pragma once

/*  
    libCppGUI - A GUI library for C++11/14
    
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
#include <array>
#include "./fonts/Rasterized_font.hpp"
#include "./fonts/Bounding_box.hpp"
#include "./fonts/Rasterized_glyph_cbox.hpp"
#include "cppgui_config.hpp"
#include <stdlib.h>


namespace cppgui {

    using ushort            = unsigned short;
    using uint              = unsigned int;
    using ulong             = unsigned long;

    using Index             = int; // Index is signed so that -1 can be used to report "invalid" or "not found"
    using Count             = unsigned int;

    using Rasterized_font   = fonts::Rasterized_font;
    using Font_size         = unsigned int;
    using Serialized_font   = std::pair<const uint8_t *, std::size_t>;

    using Text_bounding_box = fonts::Bounding_box;
    using Glyph_control_box = fonts::Rasterized_glyph_cbox;

    using Position          = int;
    using Position_delta    = int;
    using Length            = int;
    using Width             = Length;

    enum Key_state { pressed = 0, released = 1};


    enum Alignment {

        // Culturally defined

        cultural_minor_start,       // = left in western culture
        cultural_minor_middle,      
        cultural_minor_end,         // = right in western culture
        cultural_major_start,       // = top in western culture
        cultural_major_middle,
        cultural_major_end,         // = bottom in western culture
        
        // Geometrically defined
        left,
        horizontal_origin, origin = horizontal_origin,
        horizontal_middle, center = horizontal_middle,
        right,
        top,
        vertical_baseline, baseline = vertical_baseline,
        vertical_middle, middle = vertical_middle,
        bottom,

        _end
    };

    struct Point
    {
        Position        x, y;

        auto operator + (const Point &delta) const -> Point { return { x + delta.x, y + delta.y }; }
        auto operator - (const Point &delta) const -> Point { return { x - delta.x, y - delta.y }; }

        auto& operator += (const Point &delta) { x += delta.x, y += delta.y; return *this; }
        auto& operator -= (const Point &delta) { x -= delta.x, y -= delta.y; return *this; }
    };

    using Vector = Point;

    class Bbox;
    using Bbox_ref = Bbox &;
    using Bbox_cref = const Bbox &;

    struct Rectangle;

    using Padding = std::array<Width, 4>;

    /** Normalized RGBA (red, green, blue, alpha) color.
    */
    struct RGBA {

        // Predefined colors
        static constexpr RGBA black() { return { 0, 0, 0, 1 }; }
        static constexpr RGBA white() { return { 1, 1, 1, 1 }; }

        // Constructors
        constexpr RGBA(): RGBA { 0, 0, 0, 1 } {}
        constexpr RGBA(float r, float g, float b, float a = 1): components { r, g, b, a } {}

        // Accessors
        constexpr float r() const { return components[0]; }
        constexpr float g() const { return components[1]; }
        constexpr float b() const { return components[2]; }
        constexpr float a() const { return components[3]; }
        float& r() { return components[0]; }
        float& g() { return components[1];  }
        float& b() { return components[2]; }
        float& a() { return components[3]; }
        // Not really an accessor, but allows usage in calls to OpenGL
        constexpr operator const float * () const { return &components[0]; }

        // Operations
        auto operator /= (float dsor) -> RGBA & {
            components[0] /= dsor, components[1] /= dsor, components[2] /= dsor, components[3] /= dsor;
            return *this;
        }

        // Data
        std::array<float, 4> components;
    };

    inline auto operator + (const RGBA &color1, const RGBA &color2) -> RGBA 
    {
        return { color1.r() + color2.r(), color1.g() + color2.g(), color1.b() + color2.b(), color1.a() + color2.a() };
    }

    inline auto interpolate(const RGBA &color1, const RGBA &color2, float a) -> RGBA 
    {
        RGBA result;
        result.r() = color1.r() + a * (color2.r() - color1.r());
        result.g() = color1.g() + a * (color2.g() - color1.g());
        result.b() = color1.b() + a * (color2.b() - color1.b());
        result.a() = color1.a() + a * (color2.a() - color1.a());
        return result;
    }

    struct RGBA32 {
        uint8_t components[4];
    };

    inline constexpr auto 
        from_float(const RGBA &from) -> RGBA32
    {
        return { { uint8_t(from.r() * 255), uint8_t(from.g() * 255), uint8_t(from.b() * 255), uint8_t(from.a() * 255) } };
    }

    /* Normalized mono (greyscale) value.
    */
    using Mono = float;

    using Mono8 = uint8_t;

    // TODO: move Fraction<> to its own module

    template<typename T = unsigned int>
    struct Fraction {
        T num, den;

        Fraction(T n, T d = static_cast<T>(1)): num{n}, den{d} {}

        template<typename ResT>
        constexpr operator ResT() const
        {
            return static_cast<ResT>(static_cast<ResT>(num) / static_cast<ResT>(den));
        }
    };

    template<typename FracT, typename T>
    constexpr auto operator + (const Fraction<FracT> &op1, const T &op2) -> Fraction<FracT>
    {
        return Fraction<FracT>{ op1.num + op1.den * op2, op1.den };
    }

    template<typename FracT, typename T>
    constexpr auto operator * (const Fraction<FracT> &op1, const T &op2) 
    {
        return Fraction<FracT>{ op2 * op1.num, op1.den };
    }

    template<typename FracT, typename T>
    constexpr auto operator * (const T &op1, const Fraction<FracT> &op2) 
    {
        return op1 * op2.num / op2.den;
    }

    template<typename T>
    struct Range
    {
        T from, to;

        Range() = default;
        Range(const T &from_, const T &to_): from{from_}, to{to_} { assert(from_ <= to_); }
        Range(T &&from_, T &&to_): from{std::move(from_)}, to{std::move(to_)} { assert(from_ <= to_); }

        auto& operator = (const Range &src) { from = src.from, to = src.to; return *this; }
        auto& operator = (Range &&src) noexcept
        { from = std::move(src.from), to = std::move(src.to); return *this; }

        void define(const T &from_, const T &to_) { from = from_, to = to_; }

        constexpr auto length() const { return to - from; }
    };

    // TODO: use this everywhere!
    // TODO: rename to Simple_border ?
    // TODO: plural form ? (std::array<> for four cardinal directions)

    struct Border {
        Width       width;
        RGBA        color;
    };

    template<class NativeColorT>
    struct Mapped_border {
        Width           width;
        NativeColorT    color;
    };

    enum Widget_state
    {
        hovered = 0,
        focused,
        disabled,
        active,
        default_button
    };

    class Widget_states
    {
    public:
        auto& set(Widget_state state) { bits |= (1 << state); return *this; }
        auto& clear(Widget_state state) { bits &= ~(1 << state); return *this; }
        bool has(Widget_state state) const { return (bits & (1 << state)) != 0; }
    private:
        ushort  bits = 0;
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

