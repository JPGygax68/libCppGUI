#pragma once

#include <typeindex>
#include "Canvas.hpp"


namespace cppgui
{
    enum Style_element
    {
        none,
        standard_font
    };


    class Stylesheet
    {
    public:

        void add_font(Style_element, const Rasterized_font *);

        auto get_font(Style_element) const -> const Rasterized_font *;
        auto get_int(Style_element) const -> int;
        auto get_uint(Style_element) const -> uint;

    private:
        std::map<Style_element, const Rasterized_font *>    _fonts;
        std::map<Style_element, int>                        _ints;
        std::map<Style_element, uint>                       _uints;
    };

} // ns cppgui