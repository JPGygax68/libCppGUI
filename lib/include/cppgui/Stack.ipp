#include <iostream> // TODO: remove!

#include "./Stack.hpp"

namespace cppgui {

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
            _children_offset += children()[_first_visible_item]->extents().h;
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::scroll_down()
    {
        // TODO: real check: whether last item is fully visible
        if (_first_visible_item < (children().size() - 1))
        {
            _children_offset -= children()[_first_visible_item]->extents().h;
            _first_visible_item ++;
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
        _min_exts = {};

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            if (ext.w > _min_exts.w) _min_exts.w = ext.w;
            _min_exts.h += ext.h + 1;
        }

        _min_exts.h -= 1;

        return _min_exts;
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        int y = 0;

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            child->set_extents({_min_exts.w, ext.h});
            child->set_position({0, y});
            child->layout();
            y += ext.h + 1;
        }
    }

} // ns cppgui