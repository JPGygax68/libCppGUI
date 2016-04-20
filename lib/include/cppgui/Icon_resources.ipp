#include "./Icon_resources.hpp"

#include "all_icon_fonts.h"

namespace cppgui {
    
    template<Font_size Size>
    constexpr auto Icon_resources<Size>::glyph_font_data() -> std::pair<const uint8_t *, std::size_t>
    {
        return materialicons_regular<static_cast<Font_size>((double)Size)>::get(); // TODO: bigger size ?
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::tick_font_data() -> std::pair<const uint8_t *, std::size_t>
    {
        return materialicons_regular<static_cast<Font_size>((double)Size * 0.8)>::get();
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::tick_descr() -> Font_icon_descr
    {
        return {
            0xE876,
            { 0, 0 }, //{ - static_cast<Offset>(0.5 + 2 * static_cast<double>(Size) / 24), - static_cast<Offset>(0.5 + 3 * static_cast<double>(Size) / 24) },
            { 0, 0 }  //{ 0,   static_cast<Offset>(0.5 + 2 * static_cast<double>(Size) / 24) }
        };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::close() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE14C };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::right_arrow() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE154 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::back() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE15E };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::checkbox_empty() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE835 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::checkbox_checked() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE834 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::radiobutton_on() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE837 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::radiobutton_off() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE836 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::up_arrow() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE5C7 };
    }

    template<Font_size Size>
    constexpr auto Icon_resources<Size>::down_arrow() -> Icon_glyph
    {
        return { materialicons_regular<static_cast<Font_size>((double)Size*1.5)>::get(), 0xE5C5 };
    }

} // ns cppgui