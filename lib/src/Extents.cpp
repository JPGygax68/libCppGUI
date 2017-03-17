#include <Bbox.hpp>
#include <Extents.hpp>


namespace cppgui {

    Extents::Extents(Bbox_cref b)
    {
        w = -b.x_min + b.x_max;
        h = b.y_max - b.y_min;
    }

    auto Extents::operator|=(const Extents &e) -> Extents &
    {
        if (e.w > w) w = e.w;
        if (e.h > h) h = e.h;
        return *this;
    }

} // ns cppui