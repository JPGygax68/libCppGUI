#pragma once

#include <array>

#include "./basic_types.hpp"

namespace cppgui {

    using Padding = std::array<Width, 4>;

    enum Alignment {
        cultural_minor_start,       // = left in western culture
        cultural_minor_middle,      
        cultural_minor_end,         // = right in western culture
        cultural_major_start,       // = top in western culture
        cultural_major_middle,
        cultural_major_end,         // = bottom in western culture
        _end
    };

    enum Element_type {
        text,
        block
    };

    /** Layout (pseudo-) concept.
     */
    class Concept_Layout {
    public:

        void add_text_element(const Rasterized_font *, const char32_t *text, size_t len, Point *output); // { static_assert(false, "Concept_Layouter::add_text_element() not implemented"); }

        void add_block_element(const Extents &, Alignment, Point *output); // { static_assert(false, "Concept_Layouter::add_block_element() not implemented"); }

        void change_text_element(unsigned index, const Rasterized_font *, const char32_t *text, size_t len, Point *output); // { static_assert(false, "Concept_Layouter::change_text_element() not implemented"); }

        void change_block_element(unsigned index, const Extents &, Alignment, Point *output); // { static_assert(false, "Concept_Layouter::change_block_element() not implemented"); }

        void scan(); // { static_assert(false, "Concept_Layouter::scan() not implemented"); }

        void layout(const Extents &, int baseline); // { static_assert(false, "Concept_Layouter::layout() not implemented"); }
    };

    /** 
     */
    class Flow_layout: public Concept_Layout {
    public:

        void add_text_element(const Rasterized_font *, const char32_t *text, size_t len, Point *output);

        void add_block_element(const Extents &, Alignment, Point *output);

        void change_text_element(unsigned index, const Rasterized_font *, const char32_t *text, size_t len, Point *output);

        void change_block_element(unsigned index, const Extents &, Alignment, Point *output);

        void scan();

        void layout(const Extents &, int baseline);

    private:
        struct Element {
            Alignment       alignment;
            Element_type    type;
            Extents         size;
            Point       *output;
        };
    };

    /** Very simple layout that does nothing more than align a single element.

        TODO: support non-western text directions
     */
    class Single_element_layout {
    public:

        void set_text_element(const Rasterized_font *font, const char32_t *text, size_t len, Text_origin *, Rectangle *);
        void change_text_element(const Rasterized_font *font, const char32_t *text, size_t len);

        void set_minor_alignment(Alignment al) { minor_alignment = al; }
        void set_major_alignment(Alignment al) { major_alignment = al; }

        auto compute_minimal_size(/* const Padding & */) -> Extents;

        void compute_layout(const Extents & /*, const Padding & */);

    private:
        Text_bounding_box   bounding_box;
        Alignment           minor_alignment;
        Alignment           major_alignment;

        Text_origin        *text_origin = nullptr;
        Rectangle          *rectangle = nullptr;
    };

} // ns cppgui
