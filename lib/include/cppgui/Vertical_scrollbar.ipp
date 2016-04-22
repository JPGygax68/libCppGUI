#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Vertical_scrollbar<Config, With_layout>::Vertical_scrollbar()
    {
        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        add_child(&_up_btn  );
        add_child(&_down_btn);
    }

    template<class Config, bool With_layout>
    void Vertical_scrollbar<Config, With_layout>::init()
    {
        Container_t::init();

        _thumb_rect = { 2, _sliding_range.start, extents().w - 4, 20 };
    }

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

        p()->_sliding_range.start = p()->_up_btn  .rectangle().bottom() + 2;
        p()->_sliding_range.end   = p()->_down_btn.rectangle().top   () - 2;

        p()->_up_btn  .layout();
        p()->_down_btn.layout();
    }


} // ns cppgui