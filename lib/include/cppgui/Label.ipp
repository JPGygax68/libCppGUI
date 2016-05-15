#include <algorithm>

#include <gpc/fonts/rasterized_font.hpp>

#include "./layouting.hpp"

#include "./Label.hpp"

namespace cppgui {

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool With_layout>
    void Label<Config, With_layout>::init()
    {
        _font.translate( root_widget()->canvas() );
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::render(Canvas_t *cnv, const Point &offs)
    {
        fill(cnv, offs, rgba_to_native(background_color())); 

        auto pos = offs + position();
        cnv->render_text(_font.get(), pos.x + _text_origin.x, pos.y + _text_origin.y, _text.data(), _text.size());

        if (has_focus())
        {
            auto r = _text_rect.grow({3, 2});
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    Label__Layouter<Config, true, Parent>::Label__Layouter()
    {
        set_padding( default_padding() );
    }

    template<class Config, class Parent>
    void Label__Layouter<Config, true, Parent>::init_layout()
    {
        _layout.set_major_alignment(_major_alignment);
        _layout.set_minor_alignment(_minor_alignment);
        _layout.set_text_element(p()->font(), p()->_text.data(), p()->_text.size(), & p()->_text_origin, & p()->_text_rect);
    }

    template<class Config, class Parent>
    auto Label__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        assert(!p()->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        return _layout.compute_minimal_size(_padding);
    }

    template<class Config, class Parent>
    void Label__Layouter<Config, true, Parent>::layout()
    {
        _layout.compute_layout( p()->extents(), _padding);
    }

} // ns cppgui