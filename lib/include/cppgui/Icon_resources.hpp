#pragma once

#include <cstdint>
#include <utility>

#include <gpc/fonts/store.hpp>

#include "./basic_types.hpp"

namespace cppgui {

    // TODO: not required after all

    struct Font_icon_descr {
        char32_t                code_point;
        // TODO: deltas for all four properties: y_max, y_min, x_min, x_max
        Extents_delta           extents_delta;
        Position_delta          origin_delta;  // add this to the origin when rendering so that the glyph will come to rest on the baseline
    };

    struct Icon_glyph {
        gpc::fonts::Data_store  data_store;          // must be deserialized to obtain Rasterized_font
        char32_t                code_point;
    };

    template<Font_size>
    struct Icon_resources {
    
        //--- TODO: this section should be replaced

        // Generic glyph font
        static constexpr auto glyph_font_data() -> std::pair<const uint8_t *, std::size_t>; // TODO: use GSL span instead ?
                                                                                            // TODO: return both data and description in single struct ?

        static constexpr auto tick_font_data() -> std::pair<const uint8_t *, std::size_t>; // TODO: use GSL span instead ?
        static constexpr auto tick_descr() -> Font_icon_descr;

        //--- END of to-be-replaced section

        static constexpr auto close() -> Icon_glyph;
        static constexpr auto right_arrow() -> Icon_glyph;
        static constexpr auto back() -> Icon_glyph;
        static constexpr auto checkbox_empty() -> Icon_glyph;
        static constexpr auto checkbox_checked() -> Icon_glyph;
        static constexpr auto radiobutton_on () -> Icon_glyph;
        static constexpr auto radiobutton_off() -> Icon_glyph;
    };

} // ns cppgui
