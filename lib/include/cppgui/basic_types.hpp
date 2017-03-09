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


    struct Point
    {
        Position        x, y;

        auto operator + (const Point &delta) const -> Point { return { x + delta.x, y + delta.y }; }
        auto operator - (const Point &delta) const -> Point { return { x - delta.x, y - delta.y }; }

        auto& operator += (const Point &delta) { x += delta.x, y += delta.y; return *this; }
        auto& operator -= (const Point &delta) { x -= delta.x, y -= delta.y; return *this; }
    };

    using Vector = Point;

    struct Extents
    {
        Length w, h;

        explicit Extents(Length w_, Length h_): w{w_}, h{h_} {}
        Extents() = default;
    };

    class Bounding_box: public fonts::Bounding_box {
    public:
        
        /*
         * When initialized from an Extents struct, the bounding box is positioned at (0, 0),
         * the width is assigned to x_max, and the height to y_max - the ascendent - while
         * the descendent is set to 0.
         */
        explicit Bounding_box(const Extents &e): fonts::Bounding_box{0, e.w, 0, e.h} {}

        Bounding_box(const Text_bounding_box &from): Text_bounding_box{from} {}

        explicit Bounding_box(Position x_min, Position x_max, Position y_min, Position y_max):
            fonts::Bounding_box{x_min, x_max, y_min, y_max} {}

        Bounding_box() = default;

        auto expand(Width w) const -> Bounding_box
        {
            auto d = static_cast<Position_delta>(w);

            return Bounding_box{fonts::Bounding_box{ x_min - d, x_max + d, y_min - d, y_max + d }};
        }

        bool is_point_inside(const Point &p) const
        {
            #ifdef CPPGUI_Y_AXIS_DOWN

            // Important: by convention, Y is positive-down in the CppGUI coordinate system; but 
            // Y is positive-up in typography, which means that y_min extends *below* the baseline
            // with negative numbers while y_max goes grows up from the baseline with positive
            // numbers.
            return p.x >= x_min && p.x < x_max && p.y >= - y_max && p.y < - y_min;

            #else
            #error Upward Y axis not supported yet
            #endif
        }
    };

    struct Rectangle {
        Point           pos;
        Extents         ext;

        explicit Rectangle(const Bounding_box &b)
        {
            #ifdef CPPGUI_Y_AXIS_DOWN
            pos.x = b.x_min, pos.y = - b.y_max; // TODO: adapt to support positive-up Y axis
            ext.w = - b.x_min + b.x_max;
            ext.h = - b.y_min + b.y_max;
            #else
            #error Bottom-up Y axis not yet supported
            #endif
        }

        Rectangle(const Rectangle &) = default;
        Rectangle(Rectangle &&) = default;

        explicit Rectangle(const Point &p, const Extents &e): pos{p}, ext{e} {}
        explicit Rectangle(Point &&p, Extents &&e): pos{p}, ext{e} {}
        explicit Rectangle(Position x, Position y, Length w, Length h): pos{x, y}, ext{w, h} {}

        auto& operator += (const Point &p) { pos += p; return *this; }

        auto operator + (const Point &p)
        {
            auto r{*this};
            r.pos += p;
            return r;
        }

        void inflate(Width x, Width y)
        {
            pos.x -= x, ext.w += 2 * x;
            pos.y -= y, ext.h += 2 * y;
        }

        bool contains(const Point &p) const
        {
            return p.x >= pos.x && p.x < (pos.x + ext.w)
                && p.y >= pos.y && p.y < (pos.y + ext.h);
        }
    };

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

    enum Alignment {
        cultural_minor_start,       // = left in western culture
        cultural_minor_middle,      
        cultural_minor_end,         // = right in western culture
        cultural_major_start,       // = top in western culture
        cultural_major_middle,
        cultural_major_end,         // = bottom in western culture
        _end
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

