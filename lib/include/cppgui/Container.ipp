#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::set_border(const Border &border)
    {
        _border = border;
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children_resources();

        Widget_t::init();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::set_focus_to(Widget_t *child)
    {
        Abstract_container_t::set_focus_to(child);

        container()->set_focus_to(this);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::gained_focus()
    {
        container_gained_focus();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::loosing_focus()
    {
        container_loosing_focus();
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
        container_mouse_click(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_wheel(const Position &dist)
    {
        container_mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::text_input(const char32_t *text, size_t size)
    {
        container_text_input(text, size);
    }

    template<class Config, bool With_layout>
    bool Container<Config, With_layout>::handle_key_down(const Keycode &key)
    {
        // TODO: implement tab focus chaining

        return container_key_down(key);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_exit()
    {
        container_mouse_exit();
        Widget_t::mouse_exit();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::child_key_down(const Keycode & key)
    {
        if (!handle_key_down(key))
        {
            container()->child_key_down(key);
        }
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::render(Canvas_t *cv, const Position &offs)
    {
        fill(cv, offs, background_color());
        draw_borders(cv, offs, _border.width, _border.color);

        render_children(cv, offs + position());
    }

    // Container Layouter aspect ------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        for (auto child: p()->children())
        {
            child->init_layout();
        }

        recalc_minimal_size();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::recalc_minimal_size()
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

            _comp_min_size.h += _padding[0] + _padding[2];
            _comp_min_size.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];
        }
        else if (_layout_type == Layout_type::stack)
        {
            _comp_min_size.w = 0, _comp_min_size.h = 0;

            for(auto child: p()->children())
            {
                auto min_sz = child->get_minimal_size();

                // Use greatest minimal width
                if (min_sz.w > _comp_min_size.w) _comp_min_size.w = min_sz.w;

                // Add heights
                _comp_min_size.h += min_sz.h;
            }

            _comp_min_size.h += (p()->children().size() - 1) * _spacing;

            _comp_min_size.h += _padding[0] + _padding[2];
            _comp_min_size.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];
        }
        else {
            assert(false); 
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
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
        else if (_layout_type == Layout_type::stack)
        {
            compute_inner_rect();

            auto ext = p()->extents();

            Offset y = _padding[0];
            Offset x = _padding[3];

            for (auto child: p()->children())
            {
                child->set_position({ x, y });
                //child->set_extents({ ext.w - _padding[3] - _padding[1], ext });

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
        recalc_minimal_size();
        layout();
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container__Layouter<Config, true>::Aspect<Aspect_parent>::drop_child(Widget_t *child)
    {
        p()->remove_child(child);
        recalc_minimal_size();
        layout();
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default__Container__Container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui