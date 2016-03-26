#pragma once

#include "./basic_types.hpp"

namespace cppgui {

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

        void add_text_element(const Rasterized_font *, const char32_t *text, size_t len, Position *output); // { static_assert(false, "Concept_Layouter::add_text_element() not implemented"); }

        void add_block_element(const Extents &, Alignment, Position *output); // { static_assert(false, "Concept_Layouter::add_block_element() not implemented"); }

        void change_text_element(unsigned index, const Rasterized_font *, const char32_t *text, size_t len, Position *output); // { static_assert(false, "Concept_Layouter::change_text_element() not implemented"); }

        void change_block_element(unsigned index, const Extents &, Alignment, Position *output); // { static_assert(false, "Concept_Layouter::change_block_element() not implemented"); }

        void scan(); // { static_assert(false, "Concept_Layouter::scan() not implemented"); }

        void layout(const Extents &, int baseline); // { static_assert(false, "Concept_Layouter::layout() not implemented"); }
    };

    /** 
     */
    class Flow_layout: public Concept_Layout {
    public:

        void add_text_element(const Rasterized_font *, const char32_t *text, size_t len, Position *output);

        void add_block_element(const Extents &, Alignment, Position *output);

        void change_text_element(unsigned index, const Rasterized_font *, const char32_t *text, size_t len, Position *output);

        void change_block_element(unsigned index, const Extents &, Alignment, Position *output);

        void scan();

        void layout(const Extents &, int baseline);

    private:
        struct Element {
            Alignment       alignment;
            Element_type    type;
            Extents         size;
            Position       *output;
        };
    };

} // ns cppgui
