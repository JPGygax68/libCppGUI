#pragma once

#include "./basic_types.hpp"

namespace cppgui {

    struct Font_resources {
    
        // Liberation Sans

        template<int Size>
        struct liberation_sans {};

        template <>
        struct liberation_sans<24> {

            static auto font() -> const Rasterized_font *;
        };

        // MaterialIcons

        template<int Size>
        struct material_icons {};

        template <>
        struct material_icons<24> {

            static auto font() -> const Rasterized_font *;
        };
    };

} // ns cppgui
