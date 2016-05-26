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

/** TODO: this should probably be renamed to something like "Button_glyphs", 
 */

#include <cstdint>
#include <utility>

#include <gpc/fonts/store.hpp>

#include "./basic_types.hpp"

#include "all_icon_fonts.h"

namespace cppgui {

    // TODO: not required after all

    struct Font_icon_descr {
        char32_t                code_point;
        // TODO: deltas for all four properties: y_max, y_min, x_min, x_max
        Extents_delta           extents_delta;
        Vector          origin_delta;  // add this to the origin when rendering so that the glyph will come to rest on the baseline
    };

    struct Icon_glyph {
        gpc::fonts::Data_store  data_store;          // must be deserialized to obtain Rasterized_font
        char32_t                code_point;
    };

    #define CPPGUI_INSTANTIATE_ICON_RESOURCES(Font_size) \
        template cppgui::Icon_resources<Font_size>;

    template<Font_size Size>
    struct Icon_resources {
    
        //--- TODO: this section should be replaced

        // Generic glyph font
        static constexpr auto glyph_font_data() -> std::pair<const uint8_t *, std::size_t>  // TODO: use GSL span instead ?
        {
            return materialicons_regular<static_cast<Font_size>(static_cast<double>(Size))>::get(); // TODO: bigger size ?
        }
                                                                                          

        static constexpr auto tick_font_data() -> std::pair<const uint8_t *, std::size_t> // TODO: use GSL span instead ?
        {
            return materialicons_regular<static_cast<Font_size>(static_cast<double>(Size) * 0.8)>::get();
        }

        static constexpr auto tick_descr() -> Font_icon_descr
        {
            return {
                0xE876,
                { 0, 0 }, //{ - static_cast<Position>(0.5 + 2 * static_cast<double>(Size) / 24), - static_cast<Position>(0.5 + 3 * static_cast<double>(Size) / 24) },
                { 0, 0 }  //{ 0,   static_cast<Position>(0.5 + 2 * static_cast<double>(Size) / 24) }
            };
        }

        //--- END of to-be-replaced section

        static constexpr auto close() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE14C };
        }
        static constexpr auto right_arrow() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE154 };
        }
        static constexpr auto back() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE15E };
        }
        static constexpr auto checkbox_empty() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE835 };
        }
        static constexpr auto checkbox_checked() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE834 };
        }
        static constexpr auto radiobutton_on () -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE837 };
        }
        static constexpr auto radiobutton_off() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE836 };
        }
        static constexpr auto up_arrow() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE5C7 };
        }
        static constexpr auto down_arrow() -> Icon_glyph
        {
            return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE5C5 };
        }
    };

} // ns cppgui
