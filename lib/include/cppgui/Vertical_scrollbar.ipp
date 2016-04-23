#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Vertical_scrollbar<Config, With_layout>::Vertical_scrollbar()
    {
        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        add_child(&_up_btn  );
        add_child(&_down_btn);
        add_child(&_thumb);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::init()
    {
        Container_t::init();

        _thumb.set_position({ 2, _sliding_range.start });
            // TODO: position from style
    }

    /*
    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_motion(const Position &pos)
    {
        Container_t::mouse_motion(pos);

        bool on_thumb = _thumb_rect.contains(pos);

        if (on_thumb != _thumb_hovered)
        {
            _thumb_hovered = on_thumb;
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::mouse_exit()
    {
        Container_t::mouse_exit();

        if (_thumb_hovered)
        {
            _thumb_hovered = false;
            invalidate();
        }
    }
    */

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::render(Canvas_t *canvas, const Position &offset)
    {
        // Background
        // TODO: only draw the part not covered by the buttons ?
        fill(canvas, offset, canvas->rgba_to_native({0.7f, 0.7f, 0.7f, 1})); // TODO: get color from style

        /*
        // Thumb
        auto thumb_color = canvas->rgba_to_native(_thumb_hovered ? Color {0.9f, 0.9f, 0.9f, 1} : Color {0.8f, 0.8f, 0.8f, 1});
        fill_rect(canvas, _thumb_rect, offset + position(), thumb_color);
        */

        // Children: up and down buttons
        Container_t::render(canvas, offset);
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

        p()->_up_btn  .layout();
        p()->_down_btn.layout();
        p()->_thumb   .layout();
    }

    // Thumb ========================================================

    /*
    template<class Config, bool With_layout>
    class Vertical_scrollbar::Thumb: public Widget<Config, With_layout> {
    };
    */

    template<class Config, bool With_layout>
    void cppgui::Vertical_scrollbar_thumb<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        if (button == 1 && state == Key_state::pressed)
        {
            _drag_ctl.start_drag(this, pos);
        }
        else if (button == 1 && state == Key_state::released)
        {
            _drag_ctl.end_drag(this);
        }
        else
            Widget_t::mouse_button(pos, button, state);
    }

    template<class Config, bool With_layout>
    void cppgui::Vertical_scrollbar_thumb<Config, With_layout>::mouse_motion(const Position &pos)
    {
        if (Config::Mouse::is_button_down(1))
        {
            _drag_ctl.drag(this, pos);
        }
        else
            Widget_t::mouse_motion(pos);
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

} // ns cppgui