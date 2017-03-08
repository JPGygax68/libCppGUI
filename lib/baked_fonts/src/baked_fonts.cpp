#include <cppgui/fonts/store.hpp>
#include <cppgui_baked_fonts_config.hpp>
#include <baked_fonts.hpp>


namespace cppgui
{
    
    auto baked_fonts::default_font() -> const fonts::Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(CPPGUI_STANDARD_FONT_ARRAY_NAME, sizeof(CPPGUI_STANDARD_FONT_ARRAY_NAME));
            loaded = true;
        }

        return font;
    }

    auto baked_fonts::default_symbol_font() -> const fonts::Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(CPPGUI_SYMBOLS_FONT_ARRAY_NAME, sizeof(CPPGUI_SYMBOLS_FONT_ARRAY_NAME));
            loaded = true;
        }

        return font;
    }

} // ns cppgui
