#pragma once

#include <cppgui/basic_types.hpp> //fonts/rasterized_font.hpp>


namespace cppgui {
    
    namespace baked_fonts {
        
        auto default_font() -> const Rasterized_font &;
        
        //auto default_symbol_font() -> const Rasterized_font &;

        auto modernpics_18_font() -> const Rasterized_font &;

        auto modernpics_24_font() -> const Rasterized_font &;

        auto symbola_24_font() -> const Rasterized_font &;

    } // ns fonts
    
} // ns cppgui