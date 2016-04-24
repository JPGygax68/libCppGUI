#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Vertical_scrollbar<Config, With_layout>::Vertical_scrollbar()
    {
        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        add_child(&_up_btn  );
        add_child(&_down_btn);
        //add_child(&_thumb);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::on_position_change(Position_change_handler handler)
    {
        _on_position_change = handler;
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::set_value_range(const Range &range)
    {
        _position_range = range;
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::set_thumb_length(unsigned int length)
    {
        _thumb_length = length;
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::init()
    {
        Container_t::init();

        _thumb_rect.ext.h = _thumb_length * (_sliding_range.end - _sliding_range.start) / (_position_range.end - _position_range.start);

        _thumb_rect.pos = { 2, _sliding_range.start };
            // TODO: position from style
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::render(Canvas_t *canvas, const Position &offset)
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

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::change_value_range(const Range &range)
    {
        set_value_range(range);
        // TODO: recalculate position
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::change_thumb_length(unsigned int length)
    {
        set_thumb_length(length);
        // TODO: recalculate position
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        if (button == 1 && state == Key_state::pressed)
        {
            _thumb_drag_anchor_pos = pos.y;
            _thumb_drag_start_pos = _thumb_rect.pos.y;
            _dragging_thumb = true;
            root_widget()->capture_mouse(this);
        }
        else if (button == 1 && state == Key_state::released)
        {
            _dragging_thumb = false;
            root_widget()->release_mouse();
        }
        else
            Widget_t::mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_motion(const Position &pos)
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
            auto delta = pos.y - _thumb_drag_anchor_pos;
            move_thumb_to(_thumb_drag_start_pos + delta);
        }
        else
            Widget_t::mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_wheel(const Position_delta &delta)
    {
        move_thumb_to(_thumb_rect.pos.y - delta.y);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_exit()
    {
        if (_thumb_hovered)
        {
            // TODO: reaction of some kind ?
            _thumb_hovered = false;
        }
    }

    template<class Config, bool With_layout>
    auto Vertical_scrollbar<Config, With_layout>::current_position() -> Fraction<>
    {
        return { 
            static_cast<unsigned int>(_thumb_rect.pos.y - _sliding_range.start), 
            static_cast<unsigned int>(_sliding_range.end - _sliding_range.start) - _thumb_rect.ext.h
        };
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::move_thumb_to(Offset new_pos)
    {
        new_pos = std::max(new_pos, _sliding_range.start);
        new_pos = std::min(new_pos, _sliding_range.end - static_cast<Offset>(_thumb_rect.ext.h));

        _thumb_rect.pos = {2, new_pos};
        
        notify_position_change();
        invalidate();
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::notify_position_change()
    {
        if (_on_position_change) _on_position_change( current_position() );
    }

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

        p()->_sliding_range.start = p()->_up_btn  .rectangle().bottom() + 2;    // TODO: spacing from style
        p()->_sliding_range.end   = p()->_down_btn.rectangle().top   () - 2;    // TODO: spacing from style

        p()->_thumb_rect.pos.x = 2; // TODO: obtain margin from style
        p()->_thumb_rect.ext.w = p()->extents().w - 4; // obtain margin from style
        //p()->_thumb_rect.ext.h = 20; // TODO: REMOVE, temporary

        p()->_up_btn  .layout();
        p()->_down_btn.layout();
    }


    // Thumb ========================================================

    #ifdef THUMB_AS_WIDGET

    template<class Config, bool With_layout>
    void Vertical_scrollbar_thumb<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        if (button == 1 && state == Key_state::pressed)
        {
            scrollbar()->start_drag()
            _drag_ctl.start_drag(scrollbar(), pos);
            _drag_start_pos = position().y;
        }
        else if (button == 1 && state == Key_state::released)
        {
            _drag_ctl.end_drag(this);
        }
        else
            Widget_t::mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar_thumb<Config, With_layout>::render(Canvas_t *canvas, const Position & offset)
    {
        // TODO: obtain colors from style sheet
        auto color = canvas->rgba_to_native(hovered() ? Color {0.9f, 0.9f, 0.9f, 1} : Color {0.8f, 0.8f, 0.8f, 1});

        fill(canvas, offset, color);
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Vertical_scrollbar_thumb__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        // TODO ?
    }

    template<class Config>
    template<class Aspect_parent>
    auto Vertical_scrollbar_thumb__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // TODO!

        return {};
    }

    template<class Config>
    template<class Aspect_parent>
    void Vertical_scrollbar_thumb__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto scrollbar = static_cast<Vertical_scrollbar_t*>(p()->container());

        //p()->set_position({ 2, 2 });
        p()->set_extents ({ scrollbar->extents().w - 4, 10 }); // height is temporary
    }

    #endif // THUMB_AS_WIDGET

} // ns cppgui