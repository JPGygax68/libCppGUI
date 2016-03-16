#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

namespace cppgui {

    template<class Config, bool WithLayout>
    inline void Label_basic<Config, WithLayout>::set_font(const gpc::fonts::rasterized_font *font)
    {
        _font = font;
    }

    template<class Config, bool WithLayout>
    inline void Label_basic<Config, WithLayout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool WithLayout>
    inline void Label_basic<Config, WithLayout>::update_render_resources(Renderer *r)
    {
        _fnthnd = get_resource(r, _font);
    }

    template<class Config, bool WithLayout>
    inline void Label_basic<Config, WithLayout>::render(Renderer *r, const Position &offset)
    {
        fill(r, rgba_to_native(r, hovered() ? button_face_hovered_color() : button_face_color()));
        auto p = offset + position();
        r->render_text(_fnthnd, p.x + _txpos.x, p.y + _txpos.y, _text.data(), _text.size());
    }

    template<class Config>
    inline auto Label_full<Config>::minimal_size() -> Extents
    {
        assert(!_text.empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = _font->compute_text_extents(0, _text.data(), _text.size());

        return{ bounds.width(), bounds.height() };
    }

    template<class Config>
    void Label_full<Config>::layout()
    {

        auto txb = _font->compute_text_extents(0, _text.data(), _text.size());
        auto ext = extents();
        _txpos = {
            static_cast<int>((ext.w - txb.width()) / 2),
            static_cast<int>((ext.h - txb.height()) / 2 + txb.y_max)
        };
    }

} // ns cppgui