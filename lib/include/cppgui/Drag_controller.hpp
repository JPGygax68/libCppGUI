#pragma once

#include "./geometry.hpp"

namespace cppgui {

    //template<class Config, bool With_layout> class Widget;
    //template<class Config, bool With_layout> class Container;

    class Drag_controller {
    public:
        //using Widget_t = typename Widget<Config, With_layout>;
        //using Container_t = typename Container<Config, With_layout>;

        void start_drag(const Position &);
        auto drag(const Position &) -> Position_delta;
        void end_drag();

        bool dragging() const { return _dragging; }

    private:
        bool        _dragging = false;
        Position    _start_pos;
    };

} // ns cppgui