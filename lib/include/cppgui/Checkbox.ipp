#include <cassert>

#include "./Root_widget.hpp"

#include "./Checkbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::set_glyph_font_size(unsigned int size)
    {
        _glyph_font = Font_resources::material_icons<22>::font();
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::on_state_change(State_change_handler handler)
    {
        assert(!_state_change_handler);
        _state_change_handler = handler;
    }

    template<class Config, bool With_layout>
    inline void cppgui::Checkbox<Config, With_layout>::init()
    {
        _fnthnd      = root_widget()->get_font_handle(_label_font);
        _glyphfnthnd = root_widget()->get_font_handle(Font_resources::material_icons<22>::font());
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::render(Canvas_t *cv, const Position & offs)
    {
        auto pos = offs + position();

        cv->render_text(_fnthnd, pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());

        fill_rect(cv, _box_rect, pos, paper_color());
        draw_borders(cv, _box_rect, pos, stroke_width(), stroke_color());

        if (_checked)
        {
            cv->render_text(_glyphfnthnd, pos.x + _tick_pos.x, pos.y + _tick_pos.y, U"\uE876", 1);
        }
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        if (_box_rect.contains(pos)) // todo: subtract stroke_width() ?
        {
            _checked = ! _checked;
            if (_state_change_handler) _state_change_handler(_checked);
            invalidate();
        }
        else {
            Widget_t::mouse_click(pos, button, count);
        }
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_em_bounds();
        compute_label_size();
        compute_tick_size();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::compute_em_bounds()
    {
        // TODO: adapt for multi-cultural use

        _em_bounds = p()->font()->compute_text_extents(0, U"M", 1);

        _box_edge = _em_bounds.height() + 2 * (p()->margin() + p()->stroke_width());
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::compute_label_size()
    {
        _label_bounds = p()->font()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

    template<class Config>
    template<class Aspect_parent>
    void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::compute_tick_size()
    {
        _tick_bounds = p()->_glyph_font->compute_text_extents(0, U"\uE876", 1);
        // Adjustment is necessary
        _tick_bounds.y_min += 4;
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO: spacing between label and tick
        return { 
            _label_bounds.width() + _tick_bounds.width(), 
            std::max(_label_bounds.height(), _em_bounds.height() + 2 * margin() + 2 * p()->stroke_width() ) 
        };
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        int baseline = std::max(_label_bounds.y_max, _em_bounds.y_max + stroke_width() + margin() );

        p()->_label_pos = { 0,  baseline };
        // TODO: non-static glyph adjustment
        p()->_tick_pos  = { (int) (p()->extents().w - _box_edge) + p()->stroke_width() + p()->margin(), baseline + 4 };
        p()->_box_rect = { 
            p()->_tick_pos.x - stroke_width() - margin(), baseline - _em_bounds.y_max - margin() - stroke_width(),
            _box_edge, _box_edge
        };
    }

} // ns cppgui