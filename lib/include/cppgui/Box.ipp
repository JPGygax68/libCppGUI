#include "./Box.hpp"

namespace cppgui {

    template <class Renderer>
    void Box::set_header(Widget *header)
    {
        _header = header;
    }

    template<class Renderer>
    void Box<Renderer>::set_content_pane(Widget *content)
    {
        _content = content;
    }

    template<class Renderer>
    void Box<Renderer>::set_footer(Widget *footer)
    {
        _footer = footer;
    }

} // ns cppgui