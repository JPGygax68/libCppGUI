#include <iostream> // TODO: remove!

#include <gpc/fonts/store.hpp>

#include "./Stack.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Stack<Config, With_layout>::Stack()
    {
        _inner_stack.set_background_color({1, 1, 0.7f, 1}); // TODO: temporary, use style

        _up_btn  .set_font( glyph_font() );
        _down_btn.set_font( glyph_font() );

        _up_btn  .set_label(U"\x25b2");
        _down_btn.set_label(U"\x25bc");

        Abstract_container_t::add_child(&_up_btn);
        Abstract_container_t::add_child(&_down_btn);
        Abstract_container_t::add_child(&_inner_stack);
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::add_child(Widget_t *child)
    {
        _inner_stack.add_child(child);
    }

    template<class Config, bool With_layout>
    void Stack<Config, With_layout>::render(Canvas_t *cv, const Position &offs)
    {
        //auto border_ntvclr = rgba_to_native(cv, border_color());
        //draw_borders(cv, rectangle(), offs, border_width(), border_ntvclr, border_ntvclr, border_ntvclr, border_ntvclr);

        Container_t::render(cv, offs);
    }

    /*
    template<class Config, bool With_layout>
    auto Stack<Config, With_layout>::border_color() -> Color
    {
        // TODO: focus; also, get from centralized source

        if (hovered())
        {
            return {0.3f, 0.3f, 0.3f, 1};
        }
        else {
            return {0.1f, 0.1f, 0.1f, 1};
        }
    }
    */

    template<class Config, bool With_layout>
    auto Stack<Config, With_layout>::glyph_font() -> const Rasterized_font *
    {
        // TODO: adapt to font size
        static const Rasterized_font *font;
        static bool ready = false;

        if (!ready)
        {
            auto data = Config::Default_font::get();
            font = gpc::fonts::get(data.first, data.second);
            ready = true;
        }

        return font;
    }

    template<class Config, bool With_layout>
    auto Stack<Config, With_layout>::compute_children_total_size() -> Extents
    {
        Extents min_exts {};

        for (auto child: children())
        {
            auto ext = child->get_minimal_size();
            if (ext.w > min_exts.w) min_exts.w = ext.w;
            min_exts.h += ext.h + 1;
        }

        min_exts.h -= 1;

        return min_exts;
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline auto Stack__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        _min_exts = p()->compute_children_total_size();

        _min_exts.w += 2 * paper_margin();
        _min_exts.h += 2 * paper_margin();

        // TODO: also take into account sizes of up/down buttons

        return _min_exts;
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Stack__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto extents = p()->extents();
        unsigned int h_btn = p()->_up_btn.get_minimal_size().h;

        p()->_up_btn  .set_extents({extents.w, h_btn});
        p()->_down_btn.set_extents({extents.w, h_btn});
        int y1 = (int) h_btn;
        int y2 = (int) (extents.h - h_btn);
        p()->_up_btn  .set_position({0,  0});
        p()->_down_btn.set_position({0, y2});
        p()->_inner_stack.set_position({1, y1});
        p()->_inner_stack.set_extents({extents.w - 2, (unsigned int)(y2 - y1)});

        p()->layout_children(); // Container_t::layout();
    }

    // Inner_stack --------------------------------------------------

    template<class Config, bool With_layout>
    void Inner_stack<Config, With_layout>::init()
    {
        Container_t::init();

        int y = paper_margin();

        for (auto child: children())
        {
            auto ext = child->get_minimal_size();
            child->set_extents({extents().w - 2 * paper_margin(), ext.h});
            child->set_position({(int) paper_margin(), y});
            child->layout(); // will do nothing if layout aspect is nil'ed
            y += ext.h + 1;
        }

        y -= 1;
    }

    template<class Config, bool With_layout>
    void Inner_stack<Config, With_layout>::mouse_motion(const Position &pos)
    {
        // TODO: handle elements that are not "children"

        Container_t::mouse_motion(pos + Position{0, - _children_offset});

        _last_mouse_pos = pos;
    }

    template<class Config, bool With_layout>
    void Inner_stack<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        Container_t::mouse_click(pos + Position{0, - _children_offset}, button, count);
    }

    template<class Config, bool With_layout>
    inline void Inner_stack<Config, With_layout>::mouse_wheel(const Position & dist)
    {
        // std::cout << "Stack::mouse_wheel(" << dist.x << ", " << dist.y << ")" << std::endl;

        if      (dist.y < 0) scroll_down(); // TODO: use amount as parameter
        else if (dist.y > 0) scroll_up  (); // TODO: use amount as parameter
    }

    template<class Config, bool With_layout>
    void Inner_stack<Config, With_layout>::scroll_up()
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
    void Inner_stack<Config, With_layout>::scroll_down()
    {
        if (!children().empty())
        {
            auto last_child = children().back();

            if (last_child->position().y + _children_offset + last_child->extents().h > extents().h - paper_margin())
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

    template<class Config, bool With_layout>
    void Inner_stack<Config, With_layout>::render(Canvas_t *cv, const Position &offs)
    {
        fill(cv, offs, background_color());

        auto pos = offs + position();

        cv->set_clipping_rect(pos.x + paper_margin(), pos.y + paper_margin(), 
            extents().w - 2 * paper_margin(), extents().h - 2 * paper_margin());

        pos += Position{0, _children_offset};

        // TODO: optimize - do not render outside the clipping rect
        for (auto i = _first_visible_item; i < children().size(); i++)
        {
            children()[i]->render(cv, pos);
        }

        cv->cancel_clipping();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Inner_stack__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        /** TODO: this does not yet work correctly because the Container layout_children()
                strategy, and indeed the Widget class family itself, do not support a
                "preferred size".
         */
        Parent_t::layout();
    }

} // ns cppgui