#include <cppgui/Font_resources.hpp>

#include "./Fonts.hpp"

// TODO: this is a temporary stopgap. Replace with direct calls to cppgui::Font_resources

auto Fonts::default_font() -> const cppgui::Rasterized_font *
{
    return cppgui::Font_resources::liberation_sans<24>::font();
}

auto Fonts::default_glyph_font() -> const cppgui::Rasterized_font *
{
    return cppgui::Font_resources::material_icons<22>::font();
}
