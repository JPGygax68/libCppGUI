#include <iostream> // TODO: remove!

#include "./Stack.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::mouse_motion(const Position &pos)
    {
        // TODO: handle elements that are not "children"

        Container_t::mouse_motion(pos + Position{0, - _children_offset});

        _last_mouse_pos = pos;
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        // TODO: handle elements that are not "children"

        Container_t::mouse_click(pos + Position{0, - _children_offset}, button, count);
    }
    
    template<class Config, bool With_layout>
    inline void Stack<Config, With_layout>::mouse_wheel(const Position & dist)
    {
        // std::cout << "Stack::mouse_wheel(" << dist.x << ", " << dist.y << ")" << std::endl;

        if      (dist.y < 0) scroll_down(); // TODO: use amount as parameter
        else if (dist.y > 0) scroll_up  (); // TODO: use amount as parameter
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::render(Canvas_t *cv, const Position &offs)
    {
        fill(cv, offs, background_color());

        auto pos = offs + position();

        cv->set_clipping_rect(pos.x + _inner_rect.pos.x, pos.y + _inner_rect.pos.y, _inner_rect.ext.w, _inner_rect.ext.h);

        pos += Position{0, _children_offset};

        // TODO: optimize - do not render outside the clipping rect
        for (auto i = _first_visible_item; i < children().size(); i++)
        {
            children()[i]->render(cv, pos);
        }

        cv->cancel_clipping();
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::scroll_up()
    {
        if (_first_visible_item > 0)
        {
            _first_visible_item --;
            _children_offset = paper_margin() - children()[_first_visible_item]->position().y;
            mouse_motion(_last_mouse_pos); // TODO: might result in an extra invalidate()
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::scroll_down()
    {
        if (!children().empty())
        {
            auto last_child = children().back();

            if (last_child->position().y + _children_offset + last_child->extents().h > _inner_rect.pos.y + _inner_rect.ext.h)
            //if (_first_visible_item < (children().size() - 1))
            {
                _first_visible_item ++;
                _children_offset = paper_margin() - children()[_first_visible_item]->position().y;
                // TODO: if first visible child can be partially displayed, we need "bring into view" functionality
                //int empty_space = _inner_rect.pos.y + _inner_rect.ext.h - (last_child->position().y + _children_offset + last_child->extents().h);
                //if (empty_space > 0) _children_offset += empty_space;
                mouse_motion(_last_mouse_pos); // TODO: might result in an extra invalidate()
                invalidate();
            }
        }
    }

    // Layouter aspect ----------------------------------------------

    /*
    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_sizes();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        // ...
    }
    */

    template <class Config>
    template <class Aspect_parent>
    inline auto Stack_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        _min_exts = { 0, 2 * paper_margin() };

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            if (ext.w > _min_exts.w) _min_exts.w = ext.w;
            _min_exts.h += ext.h + 1;
        }

        _min_exts.h -= 1;
        _min_exts.w += 2 * paper_margin();

        return _min_exts;
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        int y = paper_margin();

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            child->set_extents({_min_exts.w - 2 * paper_margin(), ext.h});
            child->set_position({(int) paper_margin(), y});
            child->layout();
            y += ext.h + 1;
        }

        y -= 1;

        p()->_inner_rect = { 
            (int) paper_margin(), (int) paper_margin(), 
            p()->extents().w - 2 * paper_margin(), p()->extents().h - 2 * paper_margin()
        };
    }

} // ns cppgui