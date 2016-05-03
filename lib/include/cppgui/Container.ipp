#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children_resources();

        Widget_t::init();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::container_take_focus(Widget_t *child)
    {
        // Inform former focused child, update focused_child property
        Abstract_container_t::container_take_focus(child);

        // Propagate
        //container()->container_take_focus(this);
        take_focus();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::gained_focus()
    {
        if (_focused_child)
        {
            _focused_child->gained_focus();
        }

        Widget_t::gained_focus();
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::loosing_focus()
    {
        Widget_t::loosing_focus();

        if (_focused_child)
        {
            _focused_child->loosing_focus();
        }
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_motion(const Point &pos)
    {
        container_mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state)
    {
        container_mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_click(const Point &pos, int button, int count)
    {
        Widget_t::mouse_click(pos, button, count);

        container_mouse_click(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_wheel(const Vector &dist)
    {
        container_mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::text_input(const char32_t *text, size_t size)
    {
        container_text_input(text, size);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::key_down(const Keycode &key)
    {
        // If a child has focused, pass it on to that child, otherwise fall back to
        // Widget default behaviour (i.e. try to handle locally, then bubble back up)

        if (focused_child())
        {
            focused_child()->key_down(key);
        }
        else {
            Widget_t::key_down(key); // try to handle locally, bubble up if that fails
        }
    }

    template<class Config, bool With_layout>
    bool Container<Config, With_layout>::handle_key_down(const Keycode &key)
    {
        if (Config::Keyboard::is_tab(key))
        {
            if (!Config::Keyboard::is_shift_down()) return cycle_focus_forward ();  // failure will make the event bubble up
            else                                    return cycle_focus_backward();  // "
        }

        // Delegate to container key down handler
        return container_key_down(key);
    }

    template<class Config, bool With_layout>
    bool Container<Config, With_layout>::cycle_focus_forward()
    {
        assert(has_focus());

        if (children().empty())
        {
            return false; // cannot cycle, report back to sender
        }
        else
        {
            std::vector<Widget_t*>::iterator it;

            if (focused_child())
            {
                it = std::find(std::begin(children()), std::end(children()), focused_child());  
                it ++;
            }
            else {
                it = std::begin(children());
            }

            while (it != std::end(children()) && !(*it)->focussable()) it ++;

            if (it != std::end(children()))
            {
                (*it)->gained_focus();
                container_take_focus(*it);
                return true;
            }
            else {
                return false;
            }
        }
    }

    template<class Config, bool With_layout>
    bool Container<Config, With_layout>::cycle_focus_backward()
    {
        assert(has_focus());

        if (children().empty())
        {
            return false; // cannot cycle, report back to sender
        }
        else
        {
            decltype(std::rbegin(children())) it;

            if (focused_child())
            {
                it = std::find(std::rbegin(children()), std::rend(children()), focused_child());  
                it ++;
            }
            else {
                it = std::rbegin(children());
            }

            while (it != std::rend(children()) && !(*it)->focussable()) it ++;

            if (it != std::rend(children()))
            {
                (*it)->gained_focus();
                container_take_focus(*it);
                return true;
            }
            else {
                return false; // container_take_focus(nullptr);
            }

        }
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
    void Container<Config, With_layout>::render(Canvas_t *cv, const Point &offs)
    {
        fill(cv, offs, background_color());

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

            result.h += _padding[0] + _padding[2];
            result.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];
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

            result.h += _padding[0] + _padding[2];
            result.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];
        }
        else {
            assert(false); 
        }

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

            result.h += _padding[0] + _padding[2];
            result.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];
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
        result.w += (p()->children().size() - 1) * _spacing + _padding[3] + _padding[1];

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
        }
        else if (_layout_type == Layout_type::header_content)
        {
            compute_inner_rect();

            Widget_t *header  = p()->children()[0];
            Widget_t *content = p()->children()[1];

            Length h_rem = _inner_rect.ext.h; // "remaining" height
            Length h;
            Position y = _inner_rect.pos.y;
            
            h = header->get_minimal_size().h;

            header->set_position({ _inner_rect.pos.x, y });
            header->set_extents ({ _inner_rect.ext.w, h });
            y += static_cast<Position>(h);
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
            Position y = _inner_rect.bottom();

            h = footer->get_minimal_size().h;

            y -= static_cast<Position>(h);
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
            Position x = _inner_rect.right();

            w = tail->get_minimal_size().w;

            x -= static_cast<Position>(w);
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

            Position y = _padding[0];
            Position x = _padding[3];

            for (auto child: p()->children())
            {
                child->set_position({ x, y });
                child->set_extents({ ext.w - _padding[3] - _padding[1], child->get_minimal_size().h });

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
    inline void Default__Container__Container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui