#include <gpc/fonts/store.hpp>

#include <cppgui/Font_resources.hpp>

namespace cppgui {

    auto Font_resources::liberation_sans<24>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "LiberationSans-Regular-24.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

    auto Font_resources::material_icons<21>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "MaterialIcons-Regular-21.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

    auto Font_resources::material_icons<22>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "MaterialIcons-Regular-22.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

    auto Font_resources::material_icons<23>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "MaterialIcons-Regular-24.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

    auto Font_resources::material_icons<24>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "MaterialIcons-Regular-24.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

} // ns cppui