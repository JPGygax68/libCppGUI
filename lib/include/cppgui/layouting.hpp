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

        auto bounds() const -> Bounding_box;

    protected:
        void insert_separation_if_needed();

    private:
        Position_delta  _spacing = 0;
        Bounding_box    _bbox = {};
        Position        _orig = 0;
        bool            _separate = false;
    };

} // ns cppgui