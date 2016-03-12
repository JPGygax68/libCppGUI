#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

namespace cppgui {

    template<class Renderer>
    inline void Label_min<Renderer>::set_font(const gpc::fonts::rasterized_font *font)
    {
        _font = font;
    }

    template<class Renderer>
    inline void Label_min<Renderer>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Renderer>
    inline void Label_min<Renderer>::update_resources(Renderer *r)
    {
        _fnthnd = r->register_font(*_font);
    }

    template<class Renderer>
    inline void Label_min<Renderer>::render(Renderer *r, const Position &offset)
    {
        fill(r, rgba_to_native(r, {0.8f, 0.8f, 0.8f, 1}));
        auto p = offset + position();
        r->render_text(_fnthnd, p.x + _txpos.x, p.y + _txpos.y, _text.data(), _text.size());
    }

    template<class Renderer>
    inline auto Label_full<Renderer>::minimal_size() -> Extents
    {
        assert(!_text.empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = _font->compute_text_extents(0, _text.data(), _text.size());

        return{ bounds.width(), bounds.height() };
    }

    template<class Renderer>
    void Label_full<Renderer>::layout()
    {

        auto txb = _font->compute_text_extents(0, _text.data(), _text.size());
        auto ext = extents();
        _txpos = {
            static_cast<int>((ext.w - txb.width()) / 2),
            static_cast<int>((ext.h - txb.height()) / 2 + txb.y_max)
        };
    }

} // ns cppgui