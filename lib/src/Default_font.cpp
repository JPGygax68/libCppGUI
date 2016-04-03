#include <cppgui/Default_font.hpp>

#include "liberationsans_font.h"

namespace cppgui {

    auto Default_font::get() -> Serialized_font
    {
        return liberationsans_regular<size>::get();
    }

} // ns cppgui