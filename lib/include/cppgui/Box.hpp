#pragma once

#include <array>

#include "./basic_types.hpp"
#include "./layouting.hpp"
#include "./Widget.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout> struct Box__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };
    
    // Bordered_box aspect

    template<class Config, bool With_layout>
    struct Bordered_box 
    {
        template<class Aspect_parent> struct Aspect: public Aspect_parent 
        {
            using Canvas_t = typename Canvas<typename Config::Renderer>;

            void set_border(const Border &);
            // TODO: set_border( .. one setting per cardinal direction ...)

            void draw_border(Canvas_t *, const Point &offset);

        protected:
            Border      _border {1, {0, 0, 0, 1}}; // TODO: support different borders for each cardinal direction ?

            class Widget_t: public Widget<Config, With_layout> { friend struct Aspect; };
            auto p() { return static_cast<Widget<Config, With_layout>*>(this); }
        };
    };

    // Bordered_box  Layouter aspect

    template<class Config>
    struct Box__Layouter<Config, true>
    {
        template<class Aspect_parent> struct Aspect: Aspect_parent
        {
            void set_padding(Width);
            //void set_padding(const std::initializer_list<Length> &);
            void set_padding(const std::array<Width, 4> &);

            void add_padding(Rectangle &);
            auto add_padding(const Rectangle &) -> Rectangle;
            void add_padding(Extents &);
            auto add_padding(const Extents   &) -> Extents;

            Padding     _padding = { 0 };
        };
    };

} // ns cppgui
