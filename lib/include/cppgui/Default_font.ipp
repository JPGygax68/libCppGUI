#include <cppgui/Default_font.hpp>

#include "liberationsans_font.h"

namespace cppgui {

    template <Font_size Size>
    auto Default_font<Size>::get() -> Serialized_font
    {
        return liberationsans_regular<size>::get();
    }

} // ns cppgui