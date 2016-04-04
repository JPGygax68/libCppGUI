#include <algorithm>

#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

namespace cppgui {

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_font(const Rasterized_font *font)
    {
        _font = font;
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool With_layout>
    void Label<Config, With_layout>::set_background_color(const Color &clr)
    {
        _bkgnd_clr = clr;
    }

    template<class Config, bool With_layout>
    void Label<Config, With_layout>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Config, bool With_layout>
    void Label<Config, With_layout>::init()
    {
        _fnthnd = root_widget()->get_font_handle(_font);
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::render(Canvas_t *r, const Position &offs)
    {
        fill(r, offs, rgba_to_native(r, _bkgnd_clr)); 

        auto pos = offs + position();
        r->render_text(_fnthnd, pos.x + _txpos.x, pos.y + _txpos.y, _text.data(), _text.size());
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Label_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        // TODO: compute text extents here
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto Label_layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        assert(!p()->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = p()->font()->compute_text_extents(0, p()->text().data(), p()->text().size());

        return { 
            p()->_padding[3] + bounds.width () + p()->_padding[1], 
            p()->_padding[0] + bounds.height() + p()->_padding[2]
        };
    }

    template<class Config>
    template<class Aspect_parent>
    void Label_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto txb = p()->_font->compute_text_extents(0, p()->_text.data(), p()->_text.size());
        auto ext = extents();

        // TODO: select alignment

        if (_horz_align == Alignment::left)
        {
            p()->_txpos.x = 0;
        }
        else if (_horz_align == Alignment::center)
        {
            p()->_txpos.x = static_cast<Offset>((ext.w - txb.width()) / 2);
        }
        else if (_horz_align == Alignment::right)
        {
            p()->_txpos.x = static_cast<Offset>(ext.w - txb.width());
        }

        if (_vert_align == Alignment::top)
        {
            p()->_txpos.y = txb.y_max;
        }
        else if (_vert_align == Alignment::middle)
        {
            p()->_txpos.y = static_cast<Offset>((ext.h - txb.height()) / 2 + txb.y_max);
        }
        else if (_vert_align == Alignment::bottom)
        {
            p()->_txpos.y = ext.h - txb.height();
        }
    }

} // ns cppgui