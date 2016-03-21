#include "./Textbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        if (font != _font)
        {
            _font = font;
            font_changed();
        }
    }

    template<class Config, bool With_layout>
    inline void Textbox<Config, With_layout>::render(Renderer *r, const Position &offset)
    {
        fill(r, offset, rgba_to_native(r, {1, 1, 1, 1})); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::font_changed()
    {
        auto p = static_cast<Textbox<Config, true>*>(this);

        if (p->font())
        {
            // TODO: support other cultures
            auto bbox = p->font()->compute_text_extents(0, U"My", 2);
            _ascent  = bbox.y_max;
            _descent = bbox.y_min;
            _mean_char_width = (bbox.width() + 1) / 2;
        }
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Textbox_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO: replace "10" with const
        return { (unsigned) (10 * _mean_char_width), (unsigned) (_ascent - _descent) };
    };

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        _txpos = {0, _ascent };
    }

} // ns cppgui
