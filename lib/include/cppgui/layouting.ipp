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

    /*
    auto Single_element_layout::compute_minimal_size(const Padding &padding) -> Extents
    {
        return { 
            padding[3] + bounding_box.width () + padding[1], 
            padding[0] + bounding_box.height() + padding[2]
        };
    }
    */

    void Single_element_layout::compute_layout(const Extents &extents, const Padding &padding)
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

} // ns cppui