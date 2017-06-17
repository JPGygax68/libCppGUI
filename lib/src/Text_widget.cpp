#include <cppgui/Parent_map.hpp>
#include <cppgui/Text_widget.hpp>


namespace cppgui
{
    REGISTER_ANCESTOR(Widget)
    

    /*
    void Text_widget::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }
    */

    void Text_widget::obtain_style_elements()
    {
        // We do not specify a widget type index here, meaning that we are asking for a rule
        // defined for Widget.
        _font_rsrc = & get_font(standard_font);

        Widget::obtain_style_elements();
    }

    /*
    auto Text_widget::get_font(type_index Style_element e) -> Font_resource &
    {
        return _font.rasterized ? _font : Widget::get_font(e);
    }
    */

} // ns cppui