#pragma once

#include "./basic_types.hpp"
#include "./Bbox.hpp"


namespace cppgui {
    
    /*
    * Can be used to specify both the position (of the origin point) and the bounds of a 
    * Widget as a combined parameter.
    * 
    * TODO: rename to Positioned_bbox ?
    */
    struct Layout_box
    {
        Point   orig;
        Bbox    bbox;
    };

    /*
     * TODO: so far, this only serves to compute the minimal bounding box. We need actual layouting options.
     */
    class Inline_layouter
    {
    public:
        void set_default_spacing(const Rasterized_font *);
        void append_text(const Rasterized_font *, const char32_t *text, Count size);
        void append_bounding_box(Bbox_cref, bool has_border);
        void append_glyph(const Rasterized_font *, char32_t codepoint);
        void append_space();
        void separate_here();

        auto minimal_bounds() const -> Bbox;

    protected:
        void insert_separation_if_needed();

    private:
        Position_delta  _spacing = 0;
        Bbox            _bbox = {};
        Position        _orig = 0;
        bool            _separate = false;
    };

#ifdef NOT_DEFINED

    /*
     * Layout vertically, from top to bottom, stacking edge on edge, i.e. disregarding
     * typographical rules.
     */
    class Stack_layouter
    {
    public:
        void append(Bbox_cref );
        void separate(Position_delta d);

        auto minimal_bounds() const -> Bbox;
    };

#endif // NOT_DEFINED

    auto layout_element_at_right_edge(Bbox_ref container, Bbox_cref elem, Alignment valign = vertical_baseline) -> Layout_box;

    void leave_gap_at_right_edge(Bbox_ref container, Length w);

    auto layout_element_at_top_edge(Bbox_ref container, Bbox_cref elem, Alignment halign = horizontal_middle) -> Layout_box;

    void layout_gap_at_top_edge(Bbox_ref container, Length h);

    auto align_top_left(Bbox_cref cont, Bbox_cref elem) -> Layout_box;

} // ns cppgui