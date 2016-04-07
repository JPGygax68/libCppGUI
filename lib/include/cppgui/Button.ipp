#include "./Button.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    inline void cppgui::Button<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        _font = font;
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::set_label(const std::u32string &label)
    {
        _label = label;
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::init()
    {
        _fnthnd = root_widget()->get_font_handle(_font);
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::render(Canvas_t *r, const Position &offs)
    {
        fill(r, offs, rgba_to_native(r, button_face_color()));
        auto border_ntvclr = rgba_to_native(r, button_border_color());
        draw_borders(r, rectangle(), offs, button_border_width(), border_ntvclr, border_ntvclr, border_ntvclr, border_ntvclr);

        auto pos = offs + position();
        r->render_text(_fnthnd, pos.x + _label_origin.x, pos.y + _label_origin.y, _label.data(), _label.size());
    }

    // Layouter -----------------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Button_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        this->compute_bounding_box();
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto Button_layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // TODO: adjust bounding box so that it always has room for a descender (even
        // if current label does not need it) ?
        return { 
            (unsigned int) (_bbox.width () + 2 * minimal_padding()), 
            (unsigned int) (_bbox.height() + 2 * minimal_padding()) };
    }

    template<class Config>
    template<class Aspect_parent>
    void Button_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        // TODO: border, padding!

        auto ext = p()->extents();

        p()->_label_origin = {
            (int) ((ext.w - _bbox.width ()) / 2 - _bbox.x_min),
            (int) ((ext.h - _bbox.height()) / 2 + _bbox.y_max)
        };
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto cppgui::Button_layouter<Config, true>::Aspect<Aspect_parent>::minimal_padding() -> int
    {
        return 5;
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Button_layouter<Config, true>::Aspect<Aspect_parent>::font_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Button_layouter<Config, true>::Aspect<Aspect_parent>::text_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config>
    template<class Aspect_parent>
    void Button_layouter<Config, true>::Aspect<Aspect_parent>::compute_bounding_box()
    {
        _bbox = p()->_font->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

} // ns cppgui