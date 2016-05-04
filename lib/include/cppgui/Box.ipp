#include "./Box.hpp"

namespace cppgui {

    template<class Impl>
    void Bordered_box<Impl>::set_border(const Border &border)
    {
        _border = border;
    }

    template<class Impl>
    void Box__Layouter<Impl>::set_padding(Width w)
    {
        set_padding({ w, w, w, w });
    }

    /* template<class Impl>
    void Box__Layouter<Impl>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    } */

    template<class Impl>
    void Box__Layouter<Impl>::set_padding(const std::array<Length, 4> &padding)
    {
        _padding = padding; // std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Impl>
    void Box__Layouter<Impl>::add_padding(Rectangle &rect)
    {
        rect.pos.x += _padding[3] - 2;
        rect.pos.y += _padding[0] - 2;
        rect.ext.w -= _padding[3] + _padding[1] - 2 * 2;
        rect.ext.h -= _padding[0] + _padding[2] - 2 * 2;
    }

    template<class Impl>
    auto Box__Layouter<Impl>::add_padding(const Rectangle &rect) -> Rectangle
    {
        Rectangle res { rect };

        add_padding( res );

        return res;
    }

    template<class Impl>
    auto Box__Layouter<Impl>::add_padding(const Extents &ext) -> Extents
    {
        Extents res = ext;
        
        res.w += _padding[3] + _padding[1];
        res.h += _padding[0] + _padding[2];

        return res;
    }

    template<class Impl>
    void Box__Layouter<Impl>::add_padding(Extents &ext)
    {
        ext.w += _padding[3] + _padding[1];
        ext.h += _padding[0] + _padding[2];
    }

} // ns cppgui