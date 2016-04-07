#include "./Glyph_button.hpp"

namespace cppgui {

    // Main class implementation ------------------------------------

    template<class Config, bool With_layout>
    inline void Glyph_button<Config, With_layout>::init()
    {
        _font_hnd = root_widget()->get_font_handle(_glyph_fnt);
    }

    template<class Config, bool With_layout>
    void Glyph_button<Config, With_layout>::render(Canvas_t *cv, const Position & offset)
    {
        // TODO
    }


    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        obtain_glyph_bounds();
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // TODO:
        return {};
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Glyph_button__Layouter<Config, true>::Aspect<Aspect_parent>::obtain_glyph_bounds()
    {
        obtain_glyph_bounds();
    }

} // ns cppgui
