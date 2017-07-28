#pragma once

#include "./basic_types.hpp"


namespace cppgui {
        
    struct Extents
    {
        Length w, h;

        Extents(Length w_, Length h_): w{w_}, h{h_} {}
        Extents() = default;
        Extents(Bbox_cref);

        auto operator |=(const Extents &e) -> Extents &;

        auto aspect() const { return static_cast<float>(w) / static_cast<float>(h); }
    };

} // ns cppgui