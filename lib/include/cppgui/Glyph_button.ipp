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
        _font_hnd = root_widget()->get_font_handle(_glyph_fnt);
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

        cv->render_text(_font_hnd, pos.x + _glyph_pos.x, pos.y + _glyph_pos.y, &_glyph_cp, 1);
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
    inline auto Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        return { _min_edge, _min_edge };
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_glyph_pos = {
            static_cast<Offset>((p()->extents().w - _glyph_bounds.width ()) / 2), // - _glyph_bounds.x_min,
            static_cast<Offset>((p()->extents().h - _glyph_bounds.height()) / 2) + _glyph_bounds.y_max
        };
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        _glyph_bounds = p()->_glyph_fnt->lookup_glyph(0, p()->_glyph_cp)->cbox.bounds;

        _min_edge = std::max(_glyph_bounds.width() + _padding[3] + _padding[1], _glyph_bounds.height() + _padding[0] + _padding[2]);
    }

} // ns cppgui
