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
    void Label<Config, With_layout>::init()
    {
        _fnthnd = root_widget()->get_font_handle(_font);
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::render(Canvas_t *cnv, const Position &offs)
    {
        fill(cnv, offs, rgba_to_native(cnv, background_color())); 

        auto pos = offs + position();
        cnv->render_text(_fnthnd, pos.x + _text_orig.x, pos.y + _text_orig.y, _text.data(), _text.size());

        //auto r = rectangle();
        cnv->draw_stippled_rectangle_outline(pos.x + _text_rect.pos.x, pos.y + _text_rect.pos.y, 
            _text_rect.ext.w, _text_rect.ext.h, {0, 0, 0.5f, 1});
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Label__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        // TODO: compute text extents here
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto Label__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        assert(!p()->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = p()->font()->compute_text_extents(0, p()->text().data(), p()->text().size());

        return { 
            _padding[3] + bounds.width () + _padding[1], 
            _padding[0] + bounds.height() + _padding[2]
        };
    }

    template<class Config>
    template<class Aspect_parent>
    void Label__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto txb = p()->_font->compute_text_extents(0, p()->_text.data(), p()->_text.size());
        auto ext = extents();

        // TODO: select alignment
        Length w = ext.w - _padding[1] - _padding[3];
        Length h = ext.h - _padding[0] - _padding[2];

        if (_horz_align == Alignment::left)
        {
            p()->_text_orig.x = _padding[3];
        }
        else if (_horz_align == Alignment::center)
        {
            p()->_text_orig.x = static_cast<Offset>((w - txb.width()) / 2);
        }
        else if (_horz_align == Alignment::right)
        {
            p()->_text_orig.x = static_cast<Offset>(w - txb.width());
        }

        if (_vert_align == Alignment::top)
        {
            p()->_text_orig.y = _padding[0] + txb.y_max;
        }
        else if (_vert_align == Alignment::middle)
        {
            p()->_text_orig.y = static_cast<Offset>(_padding[0] + (h - txb.height()) / 2 + txb.y_max);
        }
        else if (_vert_align == Alignment::bottom)
        {
            p()->_text_orig.y = ext.h - _padding[3] + txb.y_min;
        }

        // Rectangle around text
        p()->_text_rect = {
            p()->_text_orig.x + txb.x_min, p()->_text_orig.y - txb.y_max,
            txb.width(), txb.height()
        };
    }

} // ns cppgui