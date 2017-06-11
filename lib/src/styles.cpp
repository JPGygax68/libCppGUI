#include <cppgui/styles.hpp>
#include <cppgui/baked_fonts.hpp>


namespace cppgui {

    Basic_stylesheet::Basic_stylesheet()
    {
        _font.assign(&baked_fonts::default_font());
    }

    void Basic_stylesheet::init(Canvas *c)
    {
        _font.translate(c);
    }

    auto Basic_stylesheet::font(Style_element) -> Font_resource &
    {
        return _font;
    }

} // ns cppgui