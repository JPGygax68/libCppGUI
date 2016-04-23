#include <iostream>

#include "./Drag_controller.hpp"
#include "Drag_controller.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Drag_controller<Config, With_layout>::start_drag(Widget_t *widget, const Position &pos)
    {
        assert(!_dragging);

        _start_pos = pos;

        widget->root_widget()->capture_mouse(widget);

        // TODO: send notification
        // TODO: visual feedback
        // TODO: change cursor type

        _dragging = true;
    }

    template<class Config, bool With_layout>
    void Drag_controller<Config, With_layout>::drag(Widget_t *, const Position &pos)
    {
        auto delta = pos - _start_pos;

        std::cout << "delta = " << delta.x << ", " << delta.y << std::endl;
    }

    template<class Config, bool With_layout>
    void Drag_controller<Config, With_layout>::end_drag(Widget_t *widget)
    {
        widget->root_widget()->end_mouse_capture();

        _dragging = false;
        // TODO: send notification ?

        std::cout << "Drag ended." << std::endl;
    }

} // ns cppgui