#pragma once

#include <cassert>

namespace cppgui {

    namespace fonts {
    
        struct Bounding_box {
            int x_min, x_max;   // A negative x_min represents extent before origin of first glyph
            int y_min, y_max;   // "min" is the descent and (almost?) always negative, "max" is ascent
            auto width () const { assert(x_max >= x_min); return x_max - x_min; }
            auto height() const { assert(y_max >= y_min); return y_max - y_min; }
        };

    } // ns fonts
   
} // ns cppui
