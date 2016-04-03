#pragma once

#include "./basic_types.hpp"

namespace cppgui {

    template <Font_size Size = 16>
    struct Default_font {

        static const Font_size size = Size;

        static auto get() -> Serialized_font;
    };

} // ns cppgui