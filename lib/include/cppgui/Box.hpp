#pragma once

#include "./Container.hpp"

namespace cppgui {

    template <class Config, bool With_layout> class Widget;

    template <class Config, bool With_layout>
    class Box: public Container<Config, With_layout> 
    {
    public:
        using Widget_t = Widget<Config, With_layout>;

        void set_header(Widget *);
        void set_content_pane(Widget *);
        void set_footer(Widget *);

    private:
        Widget_t *_header, *_content, *_footer;
    };

} // ns cppgui