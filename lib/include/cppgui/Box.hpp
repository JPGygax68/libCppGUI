#pragma once

#include "./basic_types.hpp"

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
        void set_padding(const std::initializer_list<Length> &);

        auto add_padding(const Rectangle &) -> Rectangle;
        auto add_padding(const Extents   &) -> Extents;

        Padding     _padding;
    };

} // ns cppgui