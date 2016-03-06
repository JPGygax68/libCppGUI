#pragma once

#include <gpc/fonts/rasterized_font.hpp>

struct Fonts {

    static auto default_font() -> gpc::fonts::rasterized_font &;
};