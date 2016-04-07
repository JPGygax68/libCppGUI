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
    void Container<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        container_mouse_button(pos, button, state);
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

        if (_layout_type == Layout_type::header_content || _layout_type == Layout_type::content_footer)
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
        else if (_layout_type == Layout_type::content_tail)
        {
            _comp_min_size.w = 0, _comp_min_size.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_minimal_size();

                // Accumulate minimal width
                _comp_min_size.w += min_sz.w;

                // Use greatest minimal height
                if (min_sz.h > _comp_min_size.h) _comp_min_size.h = min_sz.h;
            }

            _comp_min_size.w += (p()->children().size() - 1) * _spacing;
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
            compute_inner_rect();

            Widget_t *header  = p()->children()[0];
            Widget_t *content = p()->children()[1];

            Length h_rem = _inner_rect.ext.h; // "remaining" height
            Length h;
            Offset y = _inner_rect.pos.y;
            
            h = header->get_minimal_size().h;

            header->set_position({ _inner_rect.pos.x, y });
            header->set_extents ({ _inner_rect.ext.w, h });
            y += static_cast<Offset>(h);
            h_rem -= h;

            content->set_position({ _inner_rect.pos.x, y });
            content->set_extents ({ _inner_rect.ext.w, h_rem });

            for (auto child : p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::content_footer)
        {
            compute_inner_rect();

            Widget_t *content = p()->children()[0];
            Widget_t *footer  = p()->children()[1];

            Length h_rem = _inner_rect.ext.h; // "remaining" height
            Length h;
            Offset y = _inner_rect.bottom();

            h = footer->get_minimal_size().h;

            y -= static_cast<Offset>(h);
            footer->set_position({ _inner_rect.left(), y });
            footer->set_extents({ _inner_rect.width(), h });
            h_rem -= h;

            content->set_position({ _inner_rect.left(), _inner_rect.top() });
            content->set_extents({ _inner_rect.width(), h_rem });

            for (auto child : p()->children()) child->layout();
        }
        else if (_layout_type == Layout_type::content_tail)
        {
            compute_inner_rect();

            Widget_t *content = p()->children()[0];
            Widget_t *tail    = p()->children()[1];

            Length w_rem = _inner_rect.ext.w; // "remaining" width
            Length w;
            Offset x = _inner_rect.right();

            w = tail->get_minimal_size().w;

            x -= static_cast<Offset>(w);
            tail->set_position({ x, _inner_rect.top() });
            tail->set_extents({ w, _inner_rect.height() });
            w_rem -= w;

            w_rem -= _spacing;

            content->set_position({ _inner_rect.left(), _inner_rect.top() });
            content->set_extents({ w_rem, _inner_rect.height() });

            for (auto child : p()->children()) child->layout();
        }
        else {
            assert(false);
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