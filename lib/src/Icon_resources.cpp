#include <cppgui/Icon_resources.hpp>

#include "all_icon_fonts.h"

namespace cppgui {

    template <Font_size Size = 72>
    static constexpr auto get_font_data(Font_size size) -> const uint8_t *;

    constexpr auto Icon_resources::tick(Font_size size) -> const uint8_t *
    {
        return {
            get_font_data<>(size)
        };
    }

    template<Font_size Size>
    static constexpr auto get_font_data(Font_size size) -> const uint8_t *
    {
        return size == Size ? 
            materialicons_regular<Size>() :
            get_font_data<Size-1>(size);
    }

    template <> constexpr auto get_font_data<0>(Font_size) -> const uint8_t * { return nullptr; }

} // ns cppgui