#pragma once

#include <array>

#include "./basic_types.hpp"
#include "./layouting.hpp"

namespace cppgui {

    template<class Impl>
    class Bordered_box
    {
    public:

        void set_border(const Border &);
        // TODO: set_border( .. one setting per cardinal direction ...)

    protected:
        Border      _border; // TODO: support different borders for each cardinal direction ?
    };

    template<class Impl>
    struct Box__Layouter
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

} // ns cppgui