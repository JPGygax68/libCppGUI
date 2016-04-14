#include <cassert>

#include <gpc/fonts/rasterized_font.hpp>

#include "./layouting.hpp"

namespace cppgui {

    void Single_element_layout::set_text_element(const Rasterized_font * font, const char32_t * text, size_t len, 
        Text_origin *txorig, Rectangle *rect)
    {
        assert(!text_origin);
        assert(!rectangle);

        bounding_box = font->compute_text_extents(0, text, len);

        text_origin = txorig;
        rectangle = rect;
    }

    void Single_element_layout::change_text_element(const Rasterized_font * font, const char32_t * text, size_t len)
    {
        assert(text_origin);
        assert(rectangle);

        bounding_box = font->compute_text_extents(0, text, len);
    }

    auto Single_element_layout::compute_minimal_size(const Padding &padding) -> Extents
    {
        return { 
            padding[3] + bounding_box.width () + padding[1], 
            padding[0] + bounding_box.height() + padding[2]
        };
    }

    void Single_element_layout::compute_layout(const Extents &extents, const Padding &padding)
    {
        // TODO: select alignment
        Length w = extents.w - padding[1] - padding[3];
        Length h = extents.h - padding[0] - padding[2];

        if (minor_alignment == Alignment::cultural_minor_start)
        {
            text_origin->x = padding[3];
        }
        else if (minor_alignment == Alignment::cultural_minor_middle)
        {
            text_origin->x = static_cast<Offset>((w - bounding_box.width()) / 2);
        }
        else if (minor_alignment == Alignment::cultural_minor_end)
        {
            text_origin->x = static_cast<Offset>(w - bounding_box.width());
        }

        if (major_alignment == Alignment::cultural_major_start)
        {
            text_origin->y = padding[0] + bounding_box.y_max;
        }
        else if (major_alignment == Alignment::cultural_major_middle)
        {
            text_origin->y = static_cast<Offset>(padding[0] + (h - bounding_box.height()) / 2 + bounding_box.y_max);
        }
        else if (major_alignment == Alignment::cultural_major_end)
        {
            text_origin->y = extents.h - padding[3] + bounding_box.y_min;
        }

        // Rectangle around text
        *rectangle = {
            text_origin->x + bounding_box.x_min, text_origin->y - bounding_box.y_max,
            bounding_box.width(), bounding_box.height()
        };
    }

} // ns cppui