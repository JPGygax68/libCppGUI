#pragma once

#include "./Container.hpp"

namespace cppgui {

    template <class Renderer>
    class Widget;

    template <class Renderer>
    class Box: public Container<Renderer> {
    public:
        void set_header(Widget *);
        void set_content(Widget *);
        void set_footer(Widget *);

    private:
        Widget *_header, *_content, *_footer;
    };

} // ns cppgui