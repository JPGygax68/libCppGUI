#pragma once

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

    template<typename T = unsigned int>
    struct Fraction {
        T num, den;

        template<typename Float>
        constexpr auto Float() const -> Float { return static_cast<Float>(static_cast<double>(num) / static_cast<double>(den)); }
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

} // ns cppgui

namespace std {

    inline auto max(const cppgui::Extents &ext1, const cppgui::Extents &ext2) -> cppgui::Extents {

        return { max(ext1.w, ext2.w), max(ext1.h, ext2.h) };
    }

} // ns std