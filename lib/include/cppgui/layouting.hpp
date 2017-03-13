#pragma once

#include "./basic_types.hpp"


namespace cppgui {
    
    /*
     * TODO: so far, this only serves to compute the minimal bounding box. We need actual layouting options.
     */
    class Inline_layouter
    {
    public:
        void set_default_spacing(const Rasterized_font *);
        void append_text(const Rasterized_font *, const char32_t *text, Count size);
        void append_bounding_box(const Bounding_box &, bool has_border);
        void append_glyph(const Rasterized_font *, char32_t codepoint);
        void append_space();
        void separate_here();

        auto minimal_bounds() const -> Bounding_box;

    protected:
        void insert_separation_if_needed();

    private:
        Position_delta  _spacing = 0;
        Bounding_box    _bbox = {};
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
        void append(const Bounding_box &);
        void separate(Position_delta d);

        auto minimal_bounds() const -> Bounding_box;
    };

#endif // NOT_DEFINED

    auto layout_element_at_right_edge(Bbox_ref container, Bbox_cref elem, Alignment valign = vertical_baseline) -> Layout_box;

    auto layout_element_at_top_edge(Bbox_ref container, Bbox_cref elem, Alignment halign = horizontal_middle) -> Layout_box;

    auto align_top_left(Bbox_cref cont, Bbox_cref elem) -> Layout_box;

} // ns cppgui