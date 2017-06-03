#include <cppgui/Text_widget.hpp>


namespace cppgui
{
    void Text_widget::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Text_widget::init(Canvas *c)
    {
        _font.translate(c);
    }

} // ns cppui