#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

namespace cppgui {

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_font(const Rasterized_font *font)
    {
        if (font != _font)
        {
            _font = font;
            _fnthnd = root_widget()->get_font_handle(_font);
            // TODO: update layout 
        }
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::render(Renderer *r, const Position &pos)
    {
        fill(r, rgba_to_native(r, hovered() ? button_face_hovered_color() : button_face_color()));

        r->render_text(_fnthnd, pos.x + _txpos.x, pos.y + _txpos.y, _text.data(), _text.size());
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline auto Label_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        auto p = static_cast<Label<Config, true>*>(this);

        assert(!p->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = p->font()->compute_text_extents(0, p->text().data(), p->text().size());

        return{ bounds.width(), bounds.height() };
    }

    template<class Config>
    template<class Aspect_parent>
    void Label_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto p = static_cast<Label<Config, true>*>(this);

        auto txb = p->font()->compute_text_extents(0, p->text().data(), p->text().size());
        auto ext = extents();

        p->text_position() = {
            static_cast<int>((ext.w - txb.width()) / 2),
            static_cast<int>((ext.h - txb.height()) / 2 + txb.y_max)
        };
    }

} // ns cppgui