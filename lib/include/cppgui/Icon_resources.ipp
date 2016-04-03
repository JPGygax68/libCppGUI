#include "./Icon_resources.hpp"

#include "all_icon_fonts.h"

namespace cppgui {

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::tick_font_data() -> std::pair<const uint8_t *, std::size_t>
    {
        return materialicons_regular<Size - Size/10>::get();
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::tick_descr() -> Font_icon_descr
    {
        return {
            0xE876,
            { 0, - static_cast<Offset>(0.5 + 3 * static_cast<double>(Size) / 24) },
            { 0,   static_cast<Offset>(0.5 + 3 * static_cast<double>(Size) / 24) }
        };
    }

} // ns cppgui