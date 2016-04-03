#pragma once

#include "./basic_types.hpp"

namespace cppgui {

    struct Default_font {

        static const Font_size      size = 16;

        static auto get() -> Serialized_font;
    };

} // ns cppgui