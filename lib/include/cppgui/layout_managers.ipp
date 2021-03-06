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
    void layouting<Config>::Manager::set_padding(const Padding &padding)
    {
        _padding = padding;
    }

    template <class Config>
    void layouting<Config>::Manager::set_padding(Padding &&padding)
    {
        _padding = std::move(padding);
    }

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
    void layouting<Config>::Header_content::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        Widget_t *header  = cntnr.children()[0];
        Widget_t *content = cntnr.children()[1];

        Length h_rem = ext.h - this->_padding[0] - this->_padding[2];
        Length h;
        Position y = this->_padding[0];

        h = header->get_minimal_size().h;

        header->set_position({ this->_padding[3], y });
        header->set_extents ({ ext.w - this->_padding[3] - this->_padding[1], h });
        y += h + this->_spacing;
        h_rem -= h + this->_spacing;

        content->set_position({ this->_padding[3], y });
        content->set_extents ({ ext.w - this->_padding[3] - this->_padding[1], h_rem });

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
    void layouting<Config>::Content_footer::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        Widget_t *content = cntnr.children()[0];
        Widget_t *footer  = cntnr.children()[1];

        Length h_rem = ext.h - this->_padding[0] - this->_padding[2]; // "remaining" height
        Length h;
        Position y = ext.h - this->_padding[2];

        h = footer->get_minimal_size().h;

        y -= h;
        footer->set_position({ this->_padding[3], y });
        footer->set_extents ({ ext.w - this->_padding[3] - this->_padding[2], h });
        h_rem -= h;

        content->set_position({ this->_padding[3], this->_padding[0] });
        content->set_extents ({ ext.w - this->_padding[3] - this->_padding[2], h_rem });
    }

    template <class Config>
    auto layouting<Config>::Content_tail::get_minimal_size(Abstract_container_t &cntnr) -> Extents
    {
        return minimal_size_horizontal(cntnr);
    }

    template <class Config>
    void layouting<Config>::Content_tail::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        Widget_t *content = cntnr.children()[0];
        Widget_t *tail    = cntnr.children()[1];

        Length w_rem = ext.w - this->_padding[1] - this->_padding[3]; // "remaining" width
        Length w;
        Position x = ext.w - this->_padding[1];

        w = tail->get_minimal_size().w;

        x -= w;
        tail->set_position({ x, this->_padding[0] });
        tail->set_extents ({ w, ext.h - this->_padding[0] - this->_padding[2] });
        w_rem -= w;

        w_rem -= this->_spacing;

        content->set_position({ this->_padding[3], this->_padding[0] });
        content->set_extents ({ w_rem, ext.h - this->_padding[0] - this->_padding[2] });
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
    void layouting<Config>::Stack::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        Position y = 0; //_padding[0];
        Position x = 0; //_padding[3];

        for (auto child: cntnr.children())
        {
            child->set_position({ x, y });
            child->set_extents({ ext.w, child->get_minimal_size().h });

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
    void layouting<Config>::Left_to_right::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        Position x = this->_padding[3];

        auto n = 0;
        for (auto child: cntnr.children())
        {
            if (child->visible())
            {
                if (n++ > 0) x += this->_spacing;

                auto minsz = child->get_minimal_size();
                child->set_position({ x, this->_padding[0] });
                child->set_extents ({ minsz.w, ext.h - this->_padding[0] - this->_padding[2] });
            
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
    void layouting<Config>::Left_center_right::layout(Abstract_container_t &cntnr, const Extents &ext)
    {
        assert(cntnr.children().size() == 3);

        auto h_inner = ext.h - this->_padding[0] - this->_padding[2];

        // Left
        auto child = cntnr.children()[0];
        auto minsz = child->get_minimal_size();
        child->set_position({ this->_padding[3], this->_padding[0] });
        child->set_extents({ minsz.w, h_inner });
        auto x1 = this->_padding[3] + minsz.w;

        // Right
        child = cntnr.children()[2];
        minsz = child->get_minimal_size();
        child->set_position({ ext.w - this->_padding[1] - child->get_minimal_size().w });
        child->set_extents ({ minsz.w, h_inner });
        auto x2 = ext.w - minsz.w - this->_padding[1];

        // Center
        x1 += this->_spacing, x2 -= this->_spacing;
        child = cntnr.children()[1];
        child->set_position({ x1, this->_padding[1] });
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