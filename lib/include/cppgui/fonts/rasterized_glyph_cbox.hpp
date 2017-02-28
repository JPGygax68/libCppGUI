#pragma once

#include "bounding_box.hpp"

namespace cppgui {

    namespace fonts {
    
        /* Rasterized Glyph Control Box
         */         
        struct Rasterized_glyph_cbox {
            Bounding_box bounds;
            int adv_x, adv_y;
        };
        
    } // ns fonts
    
} // ns cppgui
