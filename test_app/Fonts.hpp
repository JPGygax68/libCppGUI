#pragma once

#include <cppgui/basic_types.hpp>

struct Fonts {

    static auto default_font()       -> const cppgui::Rasterized_font *;

    static auto default_glyph_font() -> const cppgui::Rasterized_font *;
};