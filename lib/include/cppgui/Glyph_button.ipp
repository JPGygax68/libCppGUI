#include "./Glyph_button.hpp"

namespace cppgui {

    // Main class implementation ------------------------------------

    template<class Config, bool With_layout>
    void Glyph_button<Config, With_layout>::set_glyph(const Icon_glyph &icgl)
    {
        _glyph_fnt = gpc::fonts::get(icgl.data_store);
        _glyph_cp  = icgl.code_point;
    }

    template<class Config, bool With_layout>
    inline void Glyph_button<Config, With_layout>::init()
    {
        if (_label_fnt) _label_font_hnd = root_widget()->get_font_handle(_label_fnt);

        _glyph_font_hnd = root_widget()->get_font_handle(_glyph_fnt);
    }

    template<class Config, bool With_layout>
    void Glyph_button<Config, With_layout>::render(Canvas_t *cv, const Position & offset)
    {
        // Background
        fill(cv, offset, rgba_to_native(cv, button_face_color()));

        // Border
        if (_border_enabled)
        {
            auto border_ntvclr = rgba_to_native(cv, button_border_color());
            draw_borders(cv, rectangle(), offset, button_border_width(), border_ntvclr, border_ntvclr, border_ntvclr, border_ntvclr);
        }

        auto pos = offset + position();

        // Label
        if (!_label.empty())
        {
            cv->render_text(_label_font_hnd, pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());
        }

        // Glyph
        cv->render_text(_glyph_font_hnd, pos.x + _glyph_pos.x, pos.y + _glyph_pos.y, &_glyph_cp, 1);
    }


    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_sizes();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        _glyph_bounds = p()->_glyph_fnt->lookup_glyph(0, p()->_glyph_cp)->cbox.bounds;

        _glyph_min_edge = std::max(_glyph_bounds.width(), _glyph_bounds.height());

        if (!p()->_label.empty())
        {
            _label_bounds = p()->_label_fnt->compute_text_extents(0, p()->_label.data(), p()->_label.size());
            _spacing = 4; // TODO: replace with value based on EM, or stylesheet setting
        }
        else {
            _label_bounds = {};
            _spacing = 0;
        }
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        return { 
            _padding[3] + _label_bounds.width () + _spacing + _glyph_min_edge + _padding[1], 
            _padding[0] + std::max(_label_bounds.height(), _glyph_min_edge)   + _padding[2]
        };
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto ext = p()->extents();

        if (!p()->_label.empty())
        {
            p()->_label_pos = {
                static_cast<Offset>(_padding[3]),
                static_cast<Offset>((ext.h - _label_bounds.height()) / 2) + _label_bounds.y_max
            };

            p()->_glyph_pos = {
                static_cast<Offset>(ext.w - _padding[1] - _glyph_min_edge),
                static_cast<Offset>((ext.h - _glyph_min_edge) / 2) + _glyph_bounds.y_max
            };
        }
        else {
            p()->_glyph_pos = {
                static_cast<Offset>((ext.w - _glyph_bounds.width ()) / 2), // - _glyph_bounds.x_min,
                static_cast<Offset>((ext.h - _glyph_bounds.height()) / 2) + _glyph_bounds.y_max
            };
        }
    }

} // ns cppgui
