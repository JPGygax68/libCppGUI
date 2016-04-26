#include <algorithm>

#include <gpc/fonts/rasterized_font.hpp>

#include "./layouting.hpp"

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
    inline void Label<Config, WithLayout>::render(Canvas_t *cnv, const Point &offs)
    {
        fill(cnv, offs, rgba_to_native(cnv, background_color())); 

        auto pos = offs + position();
        cnv->render_text(_fnthnd, pos.x + _text_origin.x, pos.y + _text_origin.y, _text.data(), _text.size());

        if (has_focus())
        {
            auto r = _text_rect.grow({3, 2});
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Label__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        _layout.set_major_alignment(_major_alignment);
        _layout.set_minor_alignment(_minor_alignment);
        _layout.set_text_element(p()->font(), p()->_text.data(), p()->_text.size(), & p()->_text_origin, & p()->_text_rect);
    }

    template<class Config>
    template<class Aspect_parent>
    inline auto Label__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        assert(!p()->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        return _layout.compute_minimal_size(_padding);
    }

    template<class Config>
    template<class Aspect_parent>
    void Label__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        _layout.compute_layout(p()->extents(), _padding);

        #ifdef NOT_DEFINED

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
            p()->_text_orig.x = static_cast<Position>((w - txb.width()) / 2);
        }
        else if (_horz_align == Alignment::right)
        {
            p()->_text_orig.x = static_cast<Position>(w - txb.width());
        }

        if (_vert_align == Alignment::top)
        {
            p()->_text_orig.y = _padding[0] + txb.y_max;
        }
        else if (_vert_align == Alignment::middle)
        {
            p()->_text_orig.y = static_cast<Position>(_padding[0] + (h - txb.height()) / 2 + txb.y_max);
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

        #endif
    }

} // ns cppgui