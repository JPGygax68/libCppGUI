#include "./layouting.hpp"

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
    void Button<Config, With_layout>::render(Canvas_t *cnv, const Position &offs)
    {
        fill(cnv, offs, rgba_to_native(cnv, button_face_color()));
        auto border_ntvclr = rgba_to_native(cnv, button_border_color());
        draw_borders(cnv, rectangle(), offs, button_border_width(), border_ntvclr, border_ntvclr, border_ntvclr, border_ntvclr);

        auto pos = offs + position();

        cnv->render_text(_fnthnd, pos.x + _label_origin.x, pos.y + _label_origin.y, _label.data(), _label.size());

        if (has_focus())
        {
            auto r = _label_rect.grow({3, 2});
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter -----------------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Button__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        // TODO: implement configurable alignment ?
        _layout.set_major_alignment(Alignment::cultural_major_middle);
        _layout.set_minor_alignment(Alignment::cultural_minor_middle);
        _layout.set_text_element(p()->font(), p()->_label.data(), p()->_label.size(), & p()->_label_origin, & p()->_label_rect);
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto Button__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        return _layout.compute_minimal_size(_padding);
    }

    template<class Config>
    template<class Aspect_parent>
    void Button__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        _layout.compute_layout(p()->extents(), _padding);
    }

    /*
    template<class Config>
    template<class Aspect_parent>
    inline auto cppgui::Button__Layouter<Config, true>::Aspect<Aspect_parent>::minimal_padding() -> int
    {
        return 5;
    }
    */

    template<class Config>
    template<class Aspect_parent>
    inline void Button__Layouter<Config, true>::Aspect<Aspect_parent>::font_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Button__Layouter<Config, true>::Aspect<Aspect_parent>::text_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config>
    template<class Aspect_parent>
    void Button__Layouter<Config, true>::Aspect<Aspect_parent>::compute_bounding_box()
    {
        _bbox = p()->_font->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

} // ns cppgui