#include "./Root_widget.hpp"

#include "./Checkbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    inline void cppgui::Checkbox<Config, With_layout>::init()
    {
        _fnthnd = root_widget()->get_font_handle(_label_font);
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::render(Canvas_t *cv, const Position & offs)
    {
        // TODO: checkbox

        auto pos = offs + position();
        cv->render_text(_fnthnd, pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_sizes();

        // TODO: determine size of "checked" glyph
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        _label_bounds = p()->font()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO
        return {};
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Checkbox_Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto txb = p()->_label_font->compute_text_extents(0, p()->_label.data(), p()->_label.size());
        auto ext = extents();

        p()->_label_pos = {
            static_cast<int>((ext.w - txb.width()) / 2),
            static_cast<int>((ext.h - txb.height()) / 2 + txb.y_max)
        };
    }

} // ns cppgui