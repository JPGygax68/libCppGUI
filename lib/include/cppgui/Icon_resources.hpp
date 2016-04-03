#pragma once

#include <cstdint>
#include <utility>

#include "./basic_types.hpp"

namespace cppgui {

    struct Font_icon_descr {
        char32_t                code_point;
        Extents_delta           extents_delta;
        Position_delta          origin_delta;  // add this to the origin when rendering so that the glyph will come to rest on the baseline
    };

    template<Font_size>
    struct Icon_resources {
    
        static constexpr auto tick_font_data() -> std::pair<const uint8_t *, std::size_t>; // TODO: use GSL span instead ?

        static constexpr auto tick_descr() -> Font_icon_descr;
    };

} // ns cppgui
