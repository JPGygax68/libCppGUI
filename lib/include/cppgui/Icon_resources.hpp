#pragma once

#include <cstdint>

#include "./basic_types.hpp"

namespace cppgui {

    struct Font_icon {
        const Rasterized_font  *font;
        char32_t                code_point;
        Extents                 pixel_size;
        Position                baseline_adjustment;  // add this to the origin when rendering so that the glyph will come to rest on the baseline
    };

    struct Icon_resources {
    
        static constexpr auto tick(Font_size size) -> const uint8_t *;

    };

} // ns cppgui
