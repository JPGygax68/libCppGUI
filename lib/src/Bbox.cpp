#include <Extents.hpp>
#include <Rectangle.hpp>
#include <Bbox.hpp>


namespace cppgui {
    
    Bbox::Bbox(const Extents &e): Text_bounding_box{0, e.w, 0, e.h}
    {        
    }

    auto Bbox::position_inside_rectangle(const Rectangle &r) const -> Point
    {
        return {
            r.pos.x - x_min + (r.ext.w - width()) / 2,
        #ifdef CPPGUI_Y_AXIS_DOWN
            r.pos.y + y_max + (r.ext.h - height()) / 2
        #else
        #error Unsupported Y axis orientation
        #endif
        };
    }

} // ns cppgui