#include "layouting.hpp"


namespace cppgui
{
    // TODO: support vertical scripts
    // TODO: testing
    
    void Inline_layouter::set_default_spacing(const Rasterized_font *f)
    {
        _spacing = f->lookup_glyph(0, U' ')->cbox.adv_x;
    }

    void Inline_layouter::append_text(const Rasterized_font *f, const char32_t *text, Count size)
    {
        insert_separation_if_needed();

        for (Count i = 0; i < size; i ++) append_glyph(f, text[i]);

        separate_here();
    }

    void Inline_layouter::append_glyph(const Rasterized_font *f, char32_t codepoint)
    {
        auto& g = f->lookup_glyph(0, codepoint)->cbox;
        _bbox.x_min = std::min(_bbox.x_min, _orig + g.bounds.x_min);
        _bbox.x_max = std::max(_bbox.x_max, _orig + g.bounds.x_max);
        _bbox.y_min = std::min(_bbox.y_min, g.bounds.y_min);
        _bbox.y_max = std::max(_bbox.y_max, g.bounds.y_max);
        _orig += g.adv_x; // TODO: vertical !
    }

    void Inline_layouter::append_space()
    {
        _orig += _spacing;
        _bbox.x_max += _spacing;
    }

    void Inline_layouter::separate_here()
    {
        _separate = true;
    }

    void Inline_layouter::append_bounding_box(const Bounding_box &b, bool has_border)
    {
        if (has_border) insert_separation_if_needed();

        _bbox.y_min = std::min(_bbox.y_min, b.y_min);
        _bbox.y_max = std::max(_bbox.y_max, b.y_max);

        _orig += b.width(); // TODO: vertical!
        _bbox.x_max += b.width();

        if (has_border) separate_here();
    }

    auto Inline_layouter::minimal_bounds() const -> Bounding_box
    {
        return _bbox;
    }

    void Inline_layouter::insert_separation_if_needed()
    {
        if (_separate)
        {
            _orig += _spacing;
            _bbox.x_max += _spacing;
            _separate = false;
        }
    }

} // ns cppgui