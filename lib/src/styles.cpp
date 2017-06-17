#include <cppgui/styles.hpp>
#include <cppgui/baked_fonts.hpp>


namespace cppgui 
{
    void Stylesheet::add_font(Style_element element, const Rasterized_font *rfont)
    {
        _fonts[element] = rfont;
    }

    auto Stylesheet::get_font(Style_element element) const -> const Rasterized_font *
    {
        return _fonts.at(element);
    }

} // ns cppgui