#pragma once

#include "Canvas.hpp"


namespace cppgui
{
    enum Style_element
    {
        none,
        standard_font
    };


    class Basic_stylesheet
    {
    public:

        Basic_stylesheet();

        void init(Canvas *);

        // void cleanup(Canvas *);

        auto font(Style_element) -> Font_resource &;

    private:
        Font_resource                   _font;
    };

} // ns cppgui