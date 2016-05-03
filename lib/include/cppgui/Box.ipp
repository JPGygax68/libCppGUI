#include "./Box.hpp"

namespace cppgui {

    template<class Impl>
    void Box__Layouter<Impl>::set_padding(Width w)
    {
        set_padding({ w, w, w, w });
    }

    template<class Impl>
    void Box__Layouter<Impl>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Impl>
    auto Box__Layouter<Impl>::add_padding(const Rectangle &) -> Rectangle
    {
        Rectangle r = p()->extents();

        r.pos.x += _padding[3] - 2;
        r.pos.y += _padding[0] - 2;
        r.ext.w -= _padding[3] + _padding[1] - 2 * 2;
        r.ext.h -= _padding[0] + _padding[2] - 2 * 2;

        return r;
    }

    template<class Impl>
    auto Box__Layouter<Impl>::add_padding(const Extents &ext) -> Extents
    {
        Extents res = ext;
        
        res.w += _padding[3] + _padding[1];
        res.h += _padding[0] + _padding[2];

        return res;
    }

} // ns cppgui