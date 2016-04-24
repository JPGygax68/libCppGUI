#include <iostream>

#include "./Drag_controller.hpp"
#include "Drag_controller.hpp"

namespace cppgui {

    void Drag_controller::start_drag(const Position &pos)
    {
        assert(!_dragging);

        _start_pos = pos;

        //root_widget()->capture_mouse(widget);

        // TODO: send notification
        // TODO: visual feedback
        // TODO: change cursor type

        _dragging = true;
    }

    auto Drag_controller::drag(const Position &pos) -> Position_delta
    {
        auto delta = pos - _start_pos;

        //std::cout << "delta = " << delta.x << ", " << delta.y << std::endl;

        return delta;
    }

    void Drag_controller::end_drag()
    {
        //widget->root_widget()->release_mouse();

        _dragging = false;
        // TODO: send notification ?

        std::cout << "Drag ended." << std::endl;
    }

} // ns cppgui