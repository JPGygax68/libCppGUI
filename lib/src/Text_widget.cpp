#include <cppgui/Text_widget.hpp>


namespace cppgui
{
    void Text_widget::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Text_widget::init(Canvas *c)
    {
        if (_font.rasterized) _font.translate(c);
    }

    auto Text_widget::get_font(Style_element e) -> Font_resource &
    {
        return _font.rasterized ? _font : Widget::get_font(e);
    }

} // ns cppui