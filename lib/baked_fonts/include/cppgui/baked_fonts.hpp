#pragma once

#include <cppgui/fonts/rasterized_font.hpp>


namespace cppgui {
    
    namespace baked_fonts {
        
        auto default_font() -> const fonts::Rasterized_font &;
        
        auto symbols_font() -> const fonts::Rasterized_font &;
        
    } // ns fonts
    
} // ns cppgui