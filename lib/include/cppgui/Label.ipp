#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

namespace cppgui {

    template<class Renderer>
    void Label<Renderer>::set_font(const gpc::fonts::rasterized_font *font)
    {
        _font = font;
    }

    template<class Renderer>
    void Label<Renderer>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Renderer>
    inline auto cppgui::Label<Renderer>::minimal_size() -> Extents
    {
        assert(!_text.empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

        auto bounds = _font->compute_text_extents(0, _text.data(), _text.size());

        return {bounds.width(), bounds.height()};
    }

    template<class Renderer>
    void Label<Renderer>::render(Renderer *r)
    {
        fill(r, rgba_to_native({0.8f, 0.8f, 0.8f, 1})); // TODO: THIS IS STRICTLY A PLACEHOLDER!!
    }

    template<class Renderer>
    inline void Label<Renderer>::Layouter::update(Label &label)
    {
        auto text_bounds = label._font->compute_text_extents(0, _text.data(), _text.size());
    }

} // ns cppgui