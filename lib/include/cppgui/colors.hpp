#pragma once

#include <cstdint>
#include <array>

namespace cppgui
{
    /** Normalized RGBA (red, green, blue, alpha) color.
    */
    struct RGBA {

        // Predefined colors
        static constexpr const RGBA black() { return { 0, 0, 0, 1 }; }
        static constexpr const RGBA white() { return { 1, 1, 1, 1 }; }

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

    inline auto 
        operator + (const RGBA &color1, const RGBA &color2) -> RGBA 
    {
        return { color1.r() + color2.r(), color1.g() + color2.g(), color1.b() + color2.b(), color1.a() + color2.a() };
    }

    inline auto
        interpolate(const RGBA &color1, const RGBA &color2, float a) -> RGBA 
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

} // ns cppgui
