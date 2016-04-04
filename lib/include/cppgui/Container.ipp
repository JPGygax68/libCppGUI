#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children_resources();

        Widget_t::init();
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_motion(const Position &pos)
    {
        container_mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        container_mouse_click(pos - position(), button, count);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_wheel(const Position &dist)
    {
        container_mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_exit()
    {
        container_mouse_exit();
        Widget_t::mouse_exit();
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::render(Canvas_t *r, const Position &offs)
    {
        // TODO: paint background, borders

        auto pos = offs + position();

        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    void Abstract_container_Layouter<Config, true>::init_children_layout()
    {
        for (auto child : p()->children())
        {
            child->init_layout();
        }
    }

    template<class Config>
    inline void Abstract_container_Layouter<Config, true>::layout_children()
    {
        // TODO: this algorithm, and the whole method, will probably become obsolete as real
        //  layouting gets implemented

        for (auto child : p()->children())
        {
            // EXPERIMENTAL: obtain minimum size and extend accordingly
            auto min_ext = child->get_minimal_size(), cur_ext = child->extents();
            if (child->extents().w == 0 && child->extents().h == 0)
            {
                child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
            }
            child->layout();
        }
    }

    // Container Layouter aspect ------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        for (auto child: p()->children())
        {
            child->init_layout();
        }

        recalc_minimal_size();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::recalc_minimal_size()
    {
        // TODO: use polymorphic delegate class 

        if (_layout_type == Layout_type::none) return;

        if (_layout_type == Layout_type::header_content)
        {
            _comp_min_size.h = 0, _comp_min_size.w = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_minimal_size();

                // Accumulate minimal height
                _comp_min_size.h += min_sz.h;

                // Use greatest minimal width
                if (min_sz.w > _comp_min_size.w) _comp_min_size.w = min_sz.w;
            }
        }
        else {
            assert(false); 
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        //p()->layout_children();

        // TODO: use polymorphic delegate class 

        if (_layout_type == Layout_type::none) 
        {
        }
        else if (_layout_type == Layout_type::header_content)
        {
            Widget_t *header  = p()->children()[0];
            Widget_t *content = p()->children()[1];

            Length h_rem = p()->extents().h; // "remaining" height
            Length h;
            Offset y = 0;
            
            h = header->get_minimal_size().h;

            header->set_position({0, 0});
            header->set_extents({ p()->extents().w, h });
            y += static_cast<Offset>(h);
            h_rem -= h;

            content->set_position({0, y});
            content->set_extents({ p()->extents().w, h_rem });

            p()->layout_children();
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::insert_child(Widget_t *child)
    {
        p()->add_child(child);
        recalc_minimal_size();
        layout();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::drop_child(Widget_t *child)
    {
        p()->remove_child(child);
        recalc_minimal_size();
        layout();
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_Container_Container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui