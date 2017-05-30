#include "layouting.hpp"


namespace cppgui
{
    // TODO: support vertical scripts
    // TODO: testing

    auto align_left(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        return (-alignee.x_min) - (-reference.x_min);
    }

    auto align_right(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        return reference.x_max - alignee.x_max;
    }

    auto align_center(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        auto w_r2 = reference.width() / 2, w_a2 = alignee.width() / 2; // halves of widths

        auto dx_r = w_r2 - (-reference.x_min); // reference origin: distance from alignment axis (positive left)
        auto dx_a = w_a2 - (-alignee  .x_min); // alignee origin: distance from alignment axis (positive left)

        return dx_r - dx_a;
    }

    auto align_top(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        return alignee.y_max - reference.y_max;
    }

    auto align_bottom(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        return - (-alignee.y_min - (-reference.y_min));
    }

    auto align_middle(Bbox_cref reference, Bbox_ref alignee) -> Position_delta
    {
        auto h_r2 = reference.height() / 2, h_a2 = alignee.height() / 2; // halves of heights

        auto dy_r = h_r2 - (-reference.y_min); // reference origin: distance from alignment axis (positive down)
        auto dy_a = h_a2 - (-alignee  .y_min); // alignee origin: distance from alignment axis (positive down)

        return dy_a - dy_r;
    }


    void Inline_layouter::set_default_spacing(const Rasterized_font *f)
    {
        _spacing = f->lookup_glyph(0, U' ')->cbox.adv_x;
    }

    void Inline_layouter::append_text(const Rasterized_font *f, const char32_t *text, Count size)
    {
        insert_separation_if_needed();

        for (Count i = 0; i < size; i ++) append_glyph(f, text[i]);

        separate_here();
    }

    void Inline_layouter::append_glyph(const Rasterized_font *f, char32_t codepoint)
    {
        auto& g = f->lookup_glyph(0, codepoint)->cbox;
        _bbox.x_min = std::min(_bbox.x_min, _orig + g.bounds.x_min);
        _bbox.x_max = std::max(_bbox.x_max, _orig + g.bounds.x_max);
        _bbox.y_min = std::min(_bbox.y_min, g.bounds.y_min);
        _bbox.y_max = std::max(_bbox.y_max, g.bounds.y_max);
        _orig += g.adv_x; // TODO: vertical !
    }

    void Inline_layouter::append_space()
    {
        _orig += _spacing;
        _bbox.x_max += _spacing;
    }

    void Inline_layouter::separate_here()
    {
        _separate = true;
    }

    void Inline_layouter::append_bounding_box(Bbox_cref b, bool has_border)
    {
        if (has_border) insert_separation_if_needed();

        _bbox.y_min = std::min(_bbox.y_min, b.y_min);
        _bbox.y_max = std::max(_bbox.y_max, b.y_max);

        _orig += b.width(); // TODO: vertical!
        _bbox.x_max += b.width();

        if (has_border) separate_here();
    }

    auto Inline_layouter::minimal_bounds() const -> Bbox
    {
        return _bbox;
    }

    void Inline_layouter::insert_separation_if_needed()
    {
        if (_separate)
        {
            _orig += _spacing;
            _bbox.x_max += _spacing;
            _separate = false;
        }
    }

    /*
     * Computes the layouting box of an element that is to be placed to the right side of a container.
     * Updates the container bounding box (subtracting the element's width from x_max), and returns
     * the layout box of the element, containing the computed position of the origin point and the
     * vertically extended bounding box to apply to the element.
     */
    auto layout_element_at_right_edge(Bbox_ref cont, Bbox_cref elem, Alignment valign) -> Layout_box
    {
        // Compute position of element origin point
        auto x = cont.x_max - elem.x_max;
    #ifdef CPPGUI_Y_AXIS_DOWN
        auto y =  elem.y_max - cont.y_max;
    #else
    #error Positive up Y axis not supported yet.
    #endif

        // Align element (vertically)
        assert(valign == Alignment::vertical_baseline);
        auto b{ elem };
        b.y_min = - (cont.y_max + (-cont.y_min) - elem.y_max);

        // Cut slice off from the right edge of the container
        cont.x_max -= elem.width();

        // Return layout box for the element
        return { { x, y }, b };
    }

    void leave_gap_at_right_edge(Bbox_ref cont, Length w)
    {
        cont.x_max -= w;
    }

    // TODO: inelegant, replace

    auto layout_element_at_top_edge(Bbox_ref cont, Bbox_cref elem, Alignment halign) -> Layout_box
    {
        // Compute position of element origin point
    #ifdef CPPGUI_Y_AXIS_DOWN
        auto y =  elem.y_max - cont.y_max;
    #else
    #error Positive up Y axis not supported yet.
    #endif

        // Horizontal alignment
        assert(halign == horizontal_middle);
        auto x = (cont.width() - elem.width()) / 2 + (-elem.x_min) - (-cont.x_min);

        // Stretch element
        auto b{ elem };
        b.x_min = cont.x_min - x;
        b.x_max = cont.x_max - x;

        // Cut slice off from top
        cont.y_max -= elem.height();

        // Return layout box for the element
        return { { x, y }, b };
    }

    // TODO: inelegant, replace

    auto layout_element_at_bottom_edge(Bbox_ref cont, Bbox_cref elem, Alignment halign) -> Layout_box
    {
        // Compute position of element origin point
    #ifdef CPPGUI_Y_AXIS_DOWN
        auto y =  cont.height() + elem.y_min - cont.y_max;
    #else
    #error Positive up Y axis not supported yet.
    #endif

        // Horizontal alignment
        assert(halign == horizontal_middle);
        auto x = (cont.width() - elem.width()) / 2 + (-elem.x_min) - (-cont.x_min);

        // Stretch element
        auto b{ elem };
        b.x_min = cont.x_min - x;
        b.x_max = cont.x_max - x;

        // Cut slice off from bottom
        cont.y_min += elem.height();

        // Return layout box for the element
        return { { x, y }, b };
    }

    void layout_gap_at_top_edge(Bbox_ref cont, Length h)
    {
        cont.y_max -= h;
    }

    void layout_gap_at_bottom_edge(Bbox_ref cont, Length h)
    {
        cont.y_min -= h;
    }

    auto align_top_left(Bbox_cref cont, Bbox_cref elem) -> Layout_box
    {
    #ifdef CPPGUI_Y_AXIS_DOWN
        return { { - (-cont.x_min - (-elem.x_min)), - (cont.y_max - elem.y_max) }, elem };
    #else
    #error Upward Y axis not supported yet
    #endif
    }

} // ns cppgui