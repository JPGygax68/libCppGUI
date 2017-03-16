#include <cppgui/basic_types.hpp>
#include <cppgui/fonts/store.hpp>
#include <cppgui_baked_fonts_config.hpp>
#include <baked_fonts.hpp>


namespace cppgui
{
    // TODO: do this with generated code (add feature to CMake target_bake_font() ?)

    auto baked_fonts::default_font() -> const Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(CPPGUI_STANDARD_FONT_ARRAY_NAME);
            loaded = true;
        }

        return font;
    }

    auto baked_fonts::modernpics_18_font() -> const Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(modernpics_18);
            loaded = true;
        }

        return font;
    }

    auto baked_fonts::modernpics_24_font() -> const Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(modernpics_24);
            loaded = true;
        }

        return font;
    }

    auto baked_fonts::symbola_24_font() -> const Rasterized_font &
    {
        static fonts::Rasterized_font font;
        static auto loaded = false;

        if (!loaded)
        {
            font = fonts::load(symbola_hint_24);
            loaded = true;
        }

        return font;
    }

} // ns cppgui
