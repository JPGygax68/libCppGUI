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

#include "./layout_managers.hpp"

#include "./Abstract_container.ipp"

namespace cppgui
{
    // TODO: all layout managers must take visible() property into account

    template <class Config>
    void layouting<Config>::Manager::set_spacing(Length spacing)
    {
        _spacing = spacing;
    }

    template <class Config>
    auto layouting<Config>::Manager::minimal_size_horizontal(Abstract_container_t &cntnr) -> Extents
    {
        Extents result; // result.w = 0, result.h = 0;

        auto n = 0;
        for (auto i = 0U; ; i++)
        {
            if (i >= cntnr.children().size()) break;

            auto child = cntnr.children()[i];

            if (child->visible())
            {
                if (n++ > 0) result.w += this->_spacing;

                auto min_sz = child->get_minimal_size();

                // Accumulate minimal width
                result.w += min_sz.w;

                // Use greatest minimal height
                if (min_sz.h > result.h) result.h = min_sz.h;
            }
        }

        // result += cntnr._padding; // TODO: use Box Layouter

        return result;
    }

    template <class Config>
    auto layouting<Config>::Header_content::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        Extents result;

        for(auto child: cntnr.children())
        {
            auto min_sz  = child->get_minimal_size();

            // Accumulate minimal, preferred height
            result.h  += min_sz.h;

            // Use greatest minimal width
            if (min_sz .w > result.w) result.w = min_sz .w;
        }

        result.h += (cntnr.children().size() - 1) * this->_spacing;

        // result += cntnr._padding;  // TODO: use Box Layouter

        return result;
    }
    
    template <class Config>
    void layouting<Config>::Header_content::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        Widget_t *header  = cntnr.children()[0];
        Widget_t *content = cntnr.children()[1];

        Length h_rem = rect.ext.h;
        Length h;
        Position y = rect.pos.y;

        h = header->get_minimal_size().h;

        header->set_position({ rect.pos.x, y });
        header->set_extents ({ rect.ext.w, h });
        y += h + this->_spacing;
        h_rem -= h + this->_spacing;

        content->set_position({ rect.pos.x, y });
        content->set_extents ({ rect.ext.w, h_rem });

        // for (auto child : cntnr.children()) child->layout(); TODO: must be done in Container_base::layout()
    }

    template <class Config>
    auto layouting<Config>::Content_footer::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        // TODO: this is identical to Header_content::get_minimal_size(): move to helper method in parent class

        Extents result;

        for(auto child: cntnr.children())
        {
            auto min_sz  = child->get_minimal_size();

            // Accumulate minimal, preferred height
            result.h  += min_sz.h;

            // Use greatest minimal width
            if (min_sz .w > result.w) result.w = min_sz .w;
        }

        // result += cntnr._padding;  // TODO: use Box Layouter

        return result;
    }

    template <class Config>
    void layouting<Config>::Content_footer::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        Widget_t *content = cntnr.children()[0];
        Widget_t *footer  = cntnr.children()[1];

        Length h_rem = rect.ext.h; // "remaining" height
        Length h;
        Position y = rect.bottom();

        h = footer->get_minimal_size().h;

        y -= h;
        footer->set_position({ rect.pos.x, y });
        footer->set_extents ({ rect.ext.w, h });
        h_rem -= h;

        content->set_position( rect.pos );
        content->set_extents ({ rect.ext.w, h_rem });
    }

    template <class Config>
    auto layouting<Config>::Content_tail::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        return minimal_size_horizontal(cntnr);
    }

    template <class Config>
    void layouting<Config>::Content_tail::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        Widget_t *content = cntnr.children()[0];
        Widget_t *tail    = cntnr.children()[1];

        Length w_rem = rect.ext.w; // "remaining" width
        Length w;
        Position x = rect.right();

        w = tail->get_minimal_size().w;

        x -= w;
        tail->set_position({ x, rect.top() });
        tail->set_extents ({ w, rect.ext.h });
        w_rem -= w;

        w_rem -= this->_spacing;

        content->set_position({ rect.pos.x, rect.pos.y });
        content->set_extents ({ w_rem, rect.ext.h });
    }

    template <class Config>
    auto layouting<Config>::Stack::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        Extents result; // result.w = 0, result.h = 0;

        for(auto child: cntnr.children())
        {
            auto min_sz = child->get_minimal_size();

            // Use greatest minimal width
            if (min_sz.w > result.w) result.w = min_sz.w;

            // Add heights
            result.h += min_sz.h;
        }

        result.h += (cntnr.children().size() - 1) * this->_spacing;

        // result += cntnr._padding;  // TODO: use Box Layouter

        return result;
    }

    template <class Config>
    void layouting<Config>::Stack::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        Position y = 0;
        Position x = 0;

        for (auto child: cntnr.children())
        {
            child->set_position({ x, y });
            child->set_extents({ rect.ext.w, child->get_minimal_size().h });

            //child->layout();

            y += child->extents().h + this->_spacing;
        }
    }

    template <class Config>
    auto layouting<Config>::Left_to_right::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        return minimal_size_horizontal(cntnr);
    }

    template <class Config>
    void layouting<Config>::Left_to_right::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        Position x = rect.left();

        auto n = 0;
        for (auto child: cntnr.children())
        {
            if (child->visible())
            {
                if (n++ > 0) x += this->_spacing;

                auto minsz = child->get_minimal_size();
                child->set_position({ x, rect.top() });
                child->set_extents ({ minsz.w, rect.ext.h });
            
                x += child->extents().w;
            }
        }
    }

    template <class Config>
    auto layouting<Config>::Left_center_right::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        assert(cntnr.children().size() == 3);

        return minimal_size_horizontal(cntnr);
    }

    template <class Config>
    void layouting<Config>::Left_center_right::layout(Abstract_container_t &cntnr, const Rectangle &rect)
    {
        assert(cntnr.children().size() == 3);

        auto h_inner = rect.ext.h;

        // Left
        auto child = cntnr.children()[0];
        auto minsz = child->get_minimal_size();
        auto x1 = rect.left();
        child->set_position({ x1, rect.top() });
        child->set_extents({ minsz.w, h_inner });

        // Right
        child = cntnr.children()[2];
        minsz = child->get_minimal_size();
        auto x2 = rect.right() - minsz.w;
        child->set_position({ x2, rect.top() });
        child->set_extents ({ minsz.w, h_inner });

        // Center
        x1 += this->_spacing, x2 -= this->_spacing;
        child = cntnr.children()[1];
        child->set_position({ x1, rect.top() });
        child->set_extents ({ x2 - x1, h_inner });
    }

    template <class Config>
    void layouting<Config>::Sequence_with_percentile_widths::set_element_length(int index, float length)
    {
        _lengths[index] = length;
    }

    template <class Config>
    auto layouting<Config>::Sequence_with_percentile_widths::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        return this->minimal_size_horizontal(cntnr);
    }

    template <class Config>
    void layouting<Config>::Sequence_with_percentile_widths::layout(Abstract_container_t &cntnr, const Extents &)
    {

    }

} // ns cppgui