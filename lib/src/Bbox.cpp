#include <Extents.hpp>
#include <Rectangle.hpp>
#include <Bbox.hpp>


namespace cppgui {
    
    Bbox::Bbox(const Extents &e, Alignment h_ref, Alignment v_ref)
    {
        x_min = 0;
        x_max = e.w;

        if (h_ref == left)
        {
            x_min = 0;
            x_max = e.w;
        }
        else if (h_ref == right)
        {
            x_min = - e.w;
            x_max = 0;
        }
        else
            assert(false);

        if (v_ref == bottom)
        {
            y_min = 0;
            y_max = e.h;            
        }
        else if (v_ref == top)
        {
            y_min = - e.h;
            y_max = 0;
        }
        else
            assert(false);
    }

    auto Bbox::collapsed() -> Bbox
    {
        using limits = std::numeric_limits<decltype(x_min)>;

        return {limits::max(), limits::min(), limits::max(), limits::min()};
    }

    /*
     * Vector from top-left corner (with CPPGUI_Y_AXIS_DOWN) to origin of bounding box.
     */
    auto Bbox::origin() const -> Vector
    {
    #ifdef CPPGUI_Y_AXIS_DOWN
        return {- x_min, y_max };
    #else
    #error Upward Y axis not supported yet
    #endif
    }

    auto Bbox::operator + (const Point &d) const -> Bbox
    {
        auto r{*this};
        r.x_min -= d.x;
#ifdef CPPGUI_Y_AXIS_DOWN
        r.y_max += d.y;
#else
        #error Upward Y axis not supported yet
#endif
        return r;
    }

    auto Bbox::expand(Width w) const -> Bbox
    {
        auto d = static_cast<Position_delta>(w);

        return {x_min - d, x_max + d, y_min - d, y_max + d};
    }

    bool Bbox::is_point_inside(const Point &p) const
    {
#ifdef CPPGUI_Y_AXIS_DOWN

        // Important: by convention, Y is positive-down in the CppGUI coordinate system; but 
        // Y is positive-up in typography, which means that y_min extends *below* the baseline
        // with negative numbers while y_max goes grows up from the baseline with positive
        // numbers.
        return p.x >= x_min && p.x < x_max && p.y >= - y_max && p.y < - y_min;

#else
        #error Upward Y axis not supported yet
#endif
    }

    auto Bbox::append_at_bottom(Bbox_cref bbox, Alignment align) -> Bbox_ref
    {
        y_min -= bbox.height();

        if (align == horizontal_middle)
        {
            if (bbox.width() > width())
            {
                auto dw = bbox.width() - width();
                auto dl = dw / 2;
                auto dr = dw - dl;
                x_min -= dl , x_max += dr;
            }
        }
        else if (align == horizontal_origin)
        {
            if (bbox.x_min < x_min) x_min = bbox.x_min;
            if (bbox.x_max > x_max) x_max = bbox.x_max;
        }

        return *this;
    }

    auto Bbox::append_at_bottom(Length h) -> Bbox_ref
    {
        y_min -= h;
        return *this;
    }

    auto Bbox::append_to_right(Bbox_cref b, Alignment valign) -> Bbox_ref
    {
        x_max += - b.x_min + b.x_max;
        // TODO: define an operation merge_vertical() ?
        if (valign == vertical_baseline)
        {
            if (b.y_max > y_max) y_max = b.y_max;
            if (b.y_min < y_min) y_min = b.y_min;
        }
        else if (valign == top)
        {
            auto h = std::max(height(), b.height());
            y_min = - (h - y_max);
        }
        else
        assert(false);

        return *this;
    }

    auto Bbox::append_to_right(Length w) -> Bbox_ref
    {
        x_max += w;
        return *this;
    }

    auto Bbox::cut_from_top(Length h) -> Bbox
    {
        auto cutoff{*this};
        y_max -= h;
        cutoff.y_min = y_max;
        return cutoff;

        //y_max -= h;
        //return *this;
    }

    auto Bbox::cut_from_bottom(Length h) -> Bbox
    {
        auto cutoff{*this};
        y_min += h;
        cutoff.y_max = y_min;
        return cutoff;
        //y_min += h;
        //return *this;
    }

    auto Bbox::cut_from_right(Length w) -> Bbox
    {
        auto cutoff{*this};
        x_max -= w;
        cutoff.x_min = x_max;
        return cutoff;
    }

    auto Bbox::merge(Bbox_cref b) -> Bbox
    {
        if (b.x_min < x_min) x_min = b.x_min;
        if (b.x_max > x_max) x_max = b.x_max;
        if (b.y_min < y_min) y_min = b.y_min;
        if (b.y_max > y_max) y_max = b.y_max;
        return *this;
    }

    auto Bbox::operator|=(Bbox_cref b) -> Bbox_ref
    {
        merge(b);
        return *this;
    }

    auto Bbox::operator|(Bbox_cref b) const -> Bbox
    {
        auto r{*this};
        r.merge(b);
        return r;
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

#ifdef SILLY_IDEA

    auto Bbox::expand_to(Bbox_cref cont, Alignment h_ref, Alignment v_ref) -> Bbox_ref
    {
        auto w_ext = cont.width()  - width ();
        auto h_ext = cont.height() - height();

        if      (h_ref == left    ) { x_max += w_ext; }
        else if (h_ref == center  ) { x_min -= w_ext / 2; x_max += cont.width() - width(); }
        else if (h_ref == right   ) { x_min -= w_ext; }
        else                        { assert(false); }

        if      (v_ref == top     ) { y_min -= h_ext; }
        else if (v_ref == middle  ) { y_min -= h_ext / 2; y_max += cont.height() - height(); }
        else if (v_ref == bottom  ) { y_max += h_ext; }
        else                        { assert(false); }

        return *this;
    }

#else

    auto Bbox::expand_to(const Extents &ext, Alignment h_ref, Alignment v_ref) -> Bbox_ref
    {
        if (ext.w > 0)
        {
            auto w_ext = ext.w - width ();
            
            if      (h_ref == left    ) { x_max += w_ext; }
            else if (h_ref == center  ) { x_min -= w_ext / 2; x_max += ext.w - width(); }
            else if (h_ref == right   ) { x_min -= w_ext; }
            else                        { assert(false); }
        }

        if (ext.h > 0)
        {
            auto h_ext = ext.h - height();

            if      (v_ref == top     ) { y_min -= h_ext; }
            else if (v_ref == middle  ) { y_min -= h_ext / 2; y_max += ext.h - height(); }
            else if (v_ref == bottom  ) { y_max += h_ext; }
            else                        { assert(false); }
        }

        return *this;
    }
#endif

} // ns cppgui