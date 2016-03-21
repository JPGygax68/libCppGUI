#include "./Textbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        if (font != _font)
        {
            _font = font;
            static_cast<Textbox_t*>(this)->font_changed();
            _fnthnd = root_widget()->get_font_handle(_font);
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::u32string &text)
    {
        _text = text;
        invalidate();
        // TODO: reposition caret
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            root_widget()->set_focus_to(this);
        }
        else {
            Widget_t::mouse_click(pos, button, count);
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::gained_focus()
    {
        // TODO: more...
        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::loosing_focus()
    {
        // TODO: more
        invalidate();
    }

    template<class Config, bool With_layout>
    inline void Textbox<Config, With_layout>::render(Renderer *r, const Position &offs)
    {
        fill(r, offs, rgba_to_native(r, {1, 1, 1, 1})); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!

        auto pos = offs + position();
        r->render_text(_fnthnd, pos.x + _txpos.x, pos.y + _txpos.y, _text.data(), _text.size());
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
        static_cast<Textbox_t*>(this)->text_position() = {0, _ascent };
    }

} // ns cppgui
