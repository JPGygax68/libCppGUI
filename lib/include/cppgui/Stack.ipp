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

        auto pos = offs + position() + Position{0, _children_offset};

        for (auto i = _first_visible_item; i < children().size(); i++)
        {
            children()[i]->render(cv, pos);
        }
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
        // TODO: real check: whether last item is fully visible
        if (_first_visible_item < (children().size() - 1))
        {
            _first_visible_item ++;
            _children_offset = paper_margin() - children()[_first_visible_item]->position().y;
            mouse_motion(_last_mouse_pos); // TODO: might result in an extra invalidate()
            invalidate();
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
    }

} // ns cppgui