#include <Bbox.hpp>
#include <Rectangle.hpp>


namespace cppgui {

    Rectangle::Rectangle(Bbox_cref b)
    {
#ifdef CPPGUI_Y_AXIS_DOWN
        pos.x = b.x_min , pos.y = - b.y_max; // TODO: adapt to support positive-up Y axis
        ext.w = - b.x_min + b.x_max;
        ext.h = - b.y_min + b.y_max;
#else
        #error Bottom-up Y axis not yet supported
#endif
    }

    Rectangle::Rectangle(Pbbox_cref pb):
        Rectangle{pb.bbox}
    {
        operator += (pb.orig);
    }

    auto Rectangle::operator+(const Point &p) -> Rectangle
    {
        auto r{*this};
        r.pos += p;
        return r;
    }

    auto Rectangle::inflate(Width x, Width y) -> Rectangle &    
    {
        pos.x -= x , ext.w += 2 * x;
        pos.y -= y , ext.h += 2 * y;
        return *this;
    }

    bool Rectangle::contains(const Point &p) const
    {
        return p.x >= x1() && p.x < x2()
            && p.y >= y1() && p.y < y2();
    }

    bool Rectangle::fully_contains(const Rectangle &r) const
    {
        return contains_full_width_of(r) && contains_full_height_of(r);
    }

    bool Rectangle::contains_full_width_of(const Rectangle &r) const
    {
        return r.x1() >= x1() && r.x2() <= x2();
    }

    bool Rectangle::contains_full_height_of(const Rectangle &r) const
    {
        return r.y1() >= y1() && r.y2() <= y2();
    }

    bool Rectangle::intersects_vertically_with(const Rectangle &r) const
    {
        // Is either edge within our borders ?
        return r.y1() > y1() && r.y1() < y2()
            || r.y2() > y1() && r.y2() < y2();
    }

} // ns cppgui