// TODO: this module is essentially obsolete, but may be reinstanted in a different form

#ifdef NOT_DEFINED

#include <gpc/fonts/store.hpp>

#include <cppgui/Font_resources.hpp>

#include "all_fonts.h"

namespace cppgui {

    auto Font_resources::liberation_sans<24>::font() -> const Rasterized_font *
    {
        static uint8_t data[] = {
            #include "LiberationSans-Regular-24.h"
        };
        static Rasterized_font font = gpc::fonts::load(data, sizeof(data));

        return &font;
    }

} // ns cppui

#endif
