#include "./Textbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        if (font != _font)
        {
        }
    }

    template<class Config, bool With_layout>
    inline void Textbox<Config, With_layout>::render(Renderer *, const Position &)
    {
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::font_changed()
    {

    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Textbox_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO
        return {};
    };

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        // TODO
    }

} // ns cppgui
