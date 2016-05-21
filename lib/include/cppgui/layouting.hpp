#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <array>

#include <gpc/fonts/rasterized_font.hpp>

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

        void set_text_element(const Rasterized_font *font, const char32_t *text, size_t len, Text_origin *txorig, Rectangle *rect)
        {
            assert(!text_origin);
            assert(!rectangle);

            bounding_box = font->compute_text_extents(0, text, len);

            text_origin = txorig;
            rectangle = rect;
        }
        void change_text_element(const Rasterized_font *font, const char32_t *text, size_t len)
        {
            assert(text_origin);
            assert(rectangle);

            bounding_box = font->compute_text_extents(0, text, len);
        }

        void set_minor_alignment(Alignment al) { minor_alignment = al; }
        void set_major_alignment(Alignment al) { major_alignment = al; }

        auto compute_minimal_size(const Padding &padding = {}) -> Extents
        {
            return { 
                padding[3] + bounding_box.width () + padding[1], 
                padding[0] + bounding_box.height() + padding[2]
            };
        }

        void compute_layout(const Extents &extents, const Padding &padding = {})
        {
            // TODO: select alignment
            Length w = extents.w - padding[1] - padding[3];
            Length h = extents.h - padding[0] - padding[2];

            if (minor_alignment == Alignment::cultural_minor_start)
            {
                text_origin->x = (Position) padding[3];
            }
            else if (minor_alignment == Alignment::cultural_minor_middle)
            {
                text_origin->x = (Position) ((w - bounding_box.width()) / 2);
            }
            else if (minor_alignment == Alignment::cultural_minor_end)
            {
                text_origin->x = (Position) (w - bounding_box.width() - padding[1]);
            }

            if (major_alignment == Alignment::cultural_major_start)
            {
                text_origin->y = padding[0] + bounding_box.y_max;
            }
            else if (major_alignment == Alignment::cultural_major_middle)
            {
                text_origin->y = (Position) (padding[0] + (h - bounding_box.height()) / 2 + bounding_box.y_max);
            }
            else if (major_alignment == Alignment::cultural_major_end)
            {
                text_origin->y = (Position) (extents.h - padding[3]) + bounding_box.y_min;
            }

            // Rectangle around text
            *rectangle = {
                text_origin->x + bounding_box.x_min, text_origin->y - bounding_box.y_max,
                bounding_box.width(), bounding_box.height()
            };
        }

    private:
        Text_bounding_box   bounding_box;
        Alignment           minor_alignment;
        Alignment           major_alignment;

        Text_origin        *text_origin = nullptr;
        Rectangle          *rectangle = nullptr;
    };

} // ns cppgui
