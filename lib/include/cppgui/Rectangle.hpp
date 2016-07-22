#pragma once

#include "./geometry.hpp"

namespace cppgui
{    
    template<Axis Ax> struct Axis_aligned_rectangle;

    template<> struct Axis_aligned_rectangle<horizontal>: Rectangle
    {
        auto& longitude() { return this->pos.x; }
        auto& length   () { return this->ext.w; }
        auto& latitude () { return this->pos.y; }
        auto& width    () { return this->ext.h; }
    };

    template<> struct Axis_aligned_rectangle<vertical>: Rectangle
    {
        auto& longitude() { return this->pos.y; }
        auto& length   () { return this->ext.h; }
        auto& latitude () { return this->pos.x; }
        auto& width    () { return this->ext.w; }
    };

    template<class Impl, bool Reversed> struct _Longitude_accessor;

    template<class Impl> 
    struct _Longitude_accessor<Impl, false>
    {
        void set_longitude_segment(Position p, Length l)
        {
            static_cast<Impl*>(this)->longitude() = p;
            static_cast<Impl*>(this)->length   () = l;
        }
    };

    template<class Impl> 
    struct _Longitude_accessor<Impl, true>
    {
        void set_longitude_segment(Position p, Length l)
        {
            static_cast<Impl*>(this)->longitude() = p - l;
            static_cast<Impl*>(this)->length   () = l;
        }
    };

    template<class Impl, bool Reversed> struct _Latitude_accessor;

    template<class Impl> 
    struct _Latitude_accessor<Impl, false>
    {
        void set_latitude_segment(Position p, Width w)
        {
            static_cast<Impl*>(this)->latitude() = p;
            static_cast<Impl*>(this)->width   () = w;
        }
    };

    /*
    template<class Impl> 
    struct _Latitude_accessor<Impl, true>
    {
        void set_latitude_segment(Position p, Width w)
        {
            static_cast<Impl*>(this)->latitude() = p + w;
            static_cast<Impl*>(this)->width   () = w;
        }
    };
    */

    template<Axis LongitudeAxis, bool LonRev> 
    struct Oriented_rectangle_base: Axis_aligned_rectangle<LongitudeAxis>,
        _Longitude_accessor< Oriented_rectangle_base<LongitudeAxis, LonRev>, LonRev>
    {
        void set_latitude_segment(Position p, Width w)
        {
            this->latitude() = p;
            this->width   () = w;
        }
    };

    template<class Config, typename = void>
    struct _vertical_axis_bottom_up
    {
        static constexpr bool value = false;
    };

    template<class Config> 
    struct _vertical_axis_bottom_up<Config, std::enable_if_t<std::is_same<const bool, decltype(Config::Renderer::y_axis_bottom_up)>::value>>
    {
        static constexpr bool value = Config::Renderer::y_axis_bottom_up;
    };

    template<class Config, Orientation Orient> 
    struct Longitude_axis_reversed
    {
        static constexpr bool value()
        {
            return 
                   Orient == right_to_left 
                || Orient == top_down  &&  _vertical_axis_bottom_up<Config>::value
                || Orient == bottom_up && !_vertical_axis_bottom_up<Config>::value;
        }
    };

    /*
    template<class Config, Orientation Orient> 
    struct Latitude_axis_reversed
    {
        static constexpr bool value()
        {
            return 
                Axis_for_orientation<Orient>::value == horizontal && ...
        }
    };
    */

    template<class Config, Orientation Orient> 
    struct Oriented_rectangle: Oriented_rectangle_base< Axis_for_orientation<Orient>::value, Longitude_axis_reversed<Config, Orient>::value() > {};

} // ns cppgui
