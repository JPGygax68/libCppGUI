#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Vertical_scrollbar<Config, With_layout>::Vertical_scrollbar()
    {
        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        _up_btn  .set_focussable(false);
        _down_btn.set_focussable(false);

        _up_btn  .on_push([this](const Point &) { move_by_elements(-1); });
        _down_btn.on_push([this](const Point &) { move_by_elements( 1); });

        add_child(&_up_btn  );
        add_child(&_down_btn);
        //add_child(&_thumb);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::on_navigation(Navigation_handler handler)
    {
        _nav_handler = handler;
    }

    /*
    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::on_position_change(Position_change_handler handler)
    {
        _on_position_change = handler;
    }
    */

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::define_range(Position full, Position shown, Position element)
    {
        _full_length = full;
        _covered_length = shown;
        _element_length = element == 0 ? shown / 10 : element;
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::init()
    {
        Container_t::init();

        _thumb_rect.pos = { 2, _sliding_range.p };
            // TODO: position from style

        recalc_thumb();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::render(Canvas_t *canvas, const Point &offset)
    {
        // Background
        // TODO: only draw the part not covered by the buttons ?
        fill(canvas, offset, canvas->rgba_to_native({0.7f, 0.7f, 0.7f, 1})); // TODO: get color from style

        // Thumb
        auto thumb_color = canvas->rgba_to_native(_thumb_hovered ? Color {0.9f, 0.9f, 0.9f, 1} : Color {0.8f, 0.8f, 0.8f, 1});
        fill_rect(canvas, _thumb_rect, offset + position(), thumb_color);

        // Children: up and down buttons
        Container_t::render(canvas, offset);
    }

    // TODO: test!!

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::change_range(Position full, Position shown, Position element)
    {
        define_range(full, shown, element);
        recalc_thumb();
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state)
    {
        if (button == 1 && state == Key_state::pressed)
        {
            if (pos.y > _sliding_range.start() && pos.y < _sliding_range.end())
            {
                auto y_rel = pos.y - _thumb_rect.pos.y;

                // Above thumb ?
                if (y_rel < 0)
                {
                    page_up();
                }
                // Below thumb ?
                else if (y_rel > _thumb_rect.ext.bottom())
                {
                    page_down();
                }
                else // On thumb
                {
                    _thumb_drag_anchor_pos = pos.y;
                    _thumb_drag_start_pos = _thumb_rect.pos.y;
                    _drag_start_pos = current_position();
                    _dragging_thumb = true;
                    root_widget()->capture_mouse(this);
                }

                return; // done with this event
            }
        }
        else if (button == 1 && state == Key_state::released)
        {
            if (_dragging_thumb)
            {
                _dragging_thumb = false;
                root_widget()->release_mouse();
            }
        }

        Container_t::mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_motion(const Point &pos)
    {
        std::cout << "mouse_motion: " << pos.x << ", " << pos.y << std::endl;

        bool on_thumb = _thumb_rect.contains(pos);
        if (on_thumb != _thumb_hovered)
        {
            _thumb_hovered = on_thumb;
            invalidate();
        }

        if (Config::Mouse::is_button_down(1) && _dragging_thumb)
        {
            notify_drag_navigation(pos.y - _thumb_drag_anchor_pos);
        }
        else
            Container_t::mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_wheel(const Vector &delta)
    {
        //move_thumb_to(_thumb_rect.pos.y - delta.y * _element_length);
        move_by_elements(-delta.y);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_exit()
    {
        if (_thumb_hovered)
        {
            // TODO: reaction of some kind ?
            _thumb_hovered = false;
        }

        Container_t::mouse_exit();
    }

    template<class Config, bool With_layout>
    auto Vertical_scrollbar<Config, With_layout>::current_position() -> Position // Fraction<>
    {
        /* return { 
            static_cast<unsigned int>(_thumb_rect.pos.y - _sliding_range.start()), 
            _sliding_range.l - _thumb_rect.ext.h
        }; */

        return _full_length * (_thumb_rect.pos.y - _sliding_range.start()) / static_cast<Position>(_sliding_range.l - _thumb_rect.ext.h);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::change_position(Position pos)
    {
        if (pos < 0) pos = 0; else if (pos > _full_length) pos = _full_length;

        _thumb_rect.pos.y = _sliding_range.p + static_cast<Position>(pos * (_sliding_range.l - _thumb_rect.ext.h) / _full_length);

        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::page_up()
    {
        //_thumb_rect.pos.y -= static_cast<Position>(_thumb_rect.ext.h);
        clip_thumb_pos();
        //notify_position_change();
        // Note: the nav handler must update the position in response
        if (_nav_handler) _nav_handler(page, current_position(), { -1, 1 });
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::page_down()
    {
        //_thumb_rect.pos.y += static_cast<Position>(_thumb_rect.ext.h);
        clip_thumb_pos();
        //notify_position_change();
        // Note: the nav handler must update the position in response
        if (_nav_handler) _nav_handler(page, current_position(), { 1, 1 });
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::move_thumb_to(Position new_pos)
    {
        new_pos = std::max(new_pos, _sliding_range.start());
        new_pos = std::min(new_pos, _sliding_range.end() - static_cast<Position>(_thumb_rect.ext.h));

        _thumb_rect.pos = {2, new_pos};
        
        //notify_position_change();
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::move_by_elements(int delta)
    {
        //_thumb_rect.pos.y += delta * static_cast<Position>((_sliding_range.l - _thumb_rect.ext.h) * _element_length / (_full_length - _shown_length));
        //clip_thumb_pos();
        //notify_position_change();
        // Note: the navigation handler must update the position in response
        if (_nav_handler) _nav_handler(Navigation_unit::element, current_position(), { delta, 1 });
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::recalc_thumb()
    {
        _thumb_rect.ext.h = _sliding_range.l * _covered_length / _full_length;

        clip_thumb_pos();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::clip_thumb_pos()
    {
        if (_thumb_rect.bottom() > _sliding_range.end())
        {
            _thumb_rect.pos.y = _sliding_range.end() - static_cast<Position>(_thumb_rect.ext.h);
        }
        else if (_thumb_rect.pos.y < _sliding_range.start())
        {
            _thumb_rect.pos.y = _sliding_range.start();
        }
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::notify_drag_navigation(Position_delta delta)
    {
        if (_nav_handler)
        {
            if (delta != 0)
            {
                _nav_handler(Navigation_unit::fraction, _drag_start_pos, { delta, static_cast<int>(_sliding_range.l) });
            }
        }
    }

    /*
    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::notify_position_change()
    {
        if (_on_position_change) _on_position_change( current_position() );
    }
    */

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Vertical_scrollbar__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto ext = p()->extents();

        auto minsz_up_btn   = p()->_up_btn  .get_minimal_size();
        auto minsz_down_btn = p()->_down_btn.get_minimal_size();

        p()->_up_btn  .set_rectangle_nw({0, 0           }, { ext.w, minsz_up_btn  .h} );
        p()->_down_btn.set_rectangle_sw({0, ext.bottom()}, { ext.w, minsz_down_btn.h} );

        p()->_sliding_range.define( p()->_up_btn.rectangle().bottom() + 2, p()->_down_btn.rectangle().top() - 2 );

        p()->_thumb_rect.pos.x = 2; // TODO: obtain margin from style
        p()->_thumb_rect.ext.w = p()->extents().w - 4; // obtain margin from style
        //p()->_thumb_rect.ext.h = 20; // TODO: REMOVE, temporary

        p()->_up_btn  .layout();
        p()->_down_btn.layout();
    }

} // ns cppgui