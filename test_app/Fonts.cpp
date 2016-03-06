#include <gpc/fonts/store.hpp>

#include "./Fonts.hpp"

auto Fonts::default_font() -> gpc::fonts::rasterized_font &
{
    static uint8_t data[] = {
        #include "LiberationSans-Regular-24.h"
    };
    static gpc::fonts::rasterized_font font = gpc::fonts::load(data, sizeof(data));

    return font;
}
