#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    inline void Container<Config, With_layout>::compute_view_from_data()
    {
        compute_child_views();

        Widget_t::compute_view_from_data();
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        for (auto child: p()->children())
        {
            child->init_layout();
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline auto Container__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // TODO: use polymorphic delegate class 

        Extents result;

        if (_layout_type == Layout_type::none)
        {            
        }
        else if (_layout_type == Layout_type::header_content || _layout_type == Layout_type::content_footer)
        {
            for(auto child: p()->children())
            {
                auto min_sz  = child->get_minimal_size();

                // Accumulate minimal, preferred height
                result.h  += min_sz.h;

                // Use greatest minimal width
                if (min_sz .w > result.w) result.w = min_sz .w;
            }
        }
        else if (_layout_type == Layout_type::content_tail)
        {
            result.w = 0, result.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_minimal_size();

                // Accumulate minimal width
                result.w += min_sz.w;

                // Use greatest minimal height
                if (min_sz.h > result.h) result.h = min_sz.h;
            }

            result.w += (p()->children().size() - 1) * _spacing;
        }
        else if (_layout_type == Layout_type::stack)
        {
            result.w = 0, result.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_minimal_size();

                // Use greatest minimal width
                if (min_sz.w > result.w) result.w = min_sz.w;

                // Add heights
                result.h += min_sz.h;
            }

            result.h += (p()->children().size() - 1) * _spacing;
        }
        else {
            assert(false); 
        }

        // Add borders
        result.h += _padding[0] + _padding[2];
        result.w += _padding[3] + _padding[1];

        return result;
    }

    template <class Config>
    template<class Aspect_parent>
    inline auto Container__Layouter<Config, true>::Aspect<Aspect_parent>::get_preferred_size() -> Extents
    {
        // TODO: use polymorphic delegate class 

        Extents result;

        if (_layout_type == Layout_type::none)
        {
        }
        else if (_layout_type == Layout_type::header_content || _layout_type == Layout_type::content_footer)
        {
            for(auto child: p()->children())
            {
                auto pref_sz = child->get_preferred_size(); 

                // Accumulate minimal, preferred height
                result.h += pref_sz.h;

                // Use greatest minimal width
                if (pref_sz.w > result.w) result.w = pref_sz.w;
            }
        }
        else if (_layout_type == Layout_type::content_tail)
        {
            result.w = 0, result.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_preferred_size();

                // Accumulate minimal width
                result.w += min_sz.w;

                // Use greatest preferred height
                if (min_sz.h > result.h) result.h = min_sz.h;
            }

            //result.h += _padding[0] + _padding[2];
            result.w += (p()->children().size() - 1) * _spacing; // + _padding[3] + _padding[1];
        }
        else if (_layout_type == Layout_type::stack)
        {
            result.w = 0, result.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_preferred_size();

                // Use greatest preferred width
                if (min_sz.w > result.w) result.w = min_sz.w;

                // Add heights
                result.h += min_sz.h;
            }

            result.h += (p()->children().size() - 1) * _spacing;

        }
        else {
            assert(false); 
        }

        // Add borders
        result.h += _padding[0] + _padding[2];
        result.w += _padding[3] + _padding[1];

        return result;
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        //p()->layout_children();

        // TODO: use polymorphic delegate class 

        if (_layout_type == Layout_type::none) 
        {
            for (auto child: p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::header_content)
        {
            auto ext = p()->extents();

            Widget_t *header  = p()->children()[0];
            Widget_t *content = p()->children()[1];

            Length h_rem = ext.h - _padding[0] - _padding[2];
            Length h;
            Position y = _padding[0];
            
            h = header->get_minimal_size().h;

            header->set_position({ (Position) _padding[3], y });
            header->set_extents ({ ext.w - _padding[3] - _padding[2], h });
            y += (Position) h;
            h_rem -= h;

            content->set_position({ (Position) _padding[3], y });
            content->set_extents ({ ext.w - _padding[3] - _padding[1], h_rem });

            for (auto child : p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::content_footer)
        {
            auto ext = p()->extents();

            Widget_t *content = p()->children()[0];
            Widget_t *footer  = p()->children()[1];

            Length h_rem = ext.h - _padding[0] - _padding[2]; // "remaining" height
            Length h;
            Position y = (Position) ext.h - _padding[2];

            h = footer->get_minimal_size().h;

            y -= static_cast<Position>(h);
            footer->set_position({ (Position) _padding[3], y });
            footer->set_extents ({ ext.w - _padding[3] - _padding[2], h });
            h_rem -= h;

            content->set_position({ (Position) _padding[3], (Position) _padding[0] });
            content->set_extents ({ ext.w - _padding[3] - _padding[2], h_rem });

            for (auto child : p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::content_tail)
        {
            auto ext = p()->extents();

            Widget_t *content = p()->children()[0];
            Widget_t *tail    = p()->children()[1];

            Length w_rem = ext.w - _padding[1] - _padding[3]; // "remaining" width
            Length w;
            Position x = (Position) ext.w - _padding[1];

            w = tail->get_minimal_size().w;

            x -= (Position) w;
            tail->set_position({ x, (Position) _padding[0] });
            tail->set_extents ({ w, ext.h - _padding[0] - _padding[2] });
            w_rem -= w;

            w_rem -= _spacing;

            content->set_position({ (Position) _padding[3], (Position) _padding[0] });
            content->set_extents ({ w_rem, ext.h - _padding[0] - _padding[2] });

            for (auto child : p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::stack)
        {
            auto ext = p()->extents();

            Position y = 0; //_padding[0];
            Position x = 0; //_padding[3];

            for (auto child: p()->children())
            {
                child->set_position({ x, y });
                child->set_extents({ ext.w, child->get_minimal_size().h });

                child->layout();

                y += child->extents().h + _spacing;
            }
        }
        else {
            assert(false);
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::insert_child(Widget_t *child)
    {
        p()->add_child(child);
        layout();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::drop_child(Widget_t *child)
    {
        p()->remove_child(child);
        layout();
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default__Container_base__Container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui