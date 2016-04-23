#pragma once

#include "./geometry.hpp"

namespace cppgui {

    template<class Config, bool With_layout> class Widget;

    template<class Config, bool With_layout>
    class Drag_controller
    {
    public:
        using Widget_t = typename Widget<Config, With_layout>;

        void start_drag(Widget_t *, const Position &);
        void drag(Widget_t *, const Position &);
        void end_drag(Widget_t *);

    private:
        bool        _dragging = false;
        Position    _start_pos;
    };

} // ns cppgui