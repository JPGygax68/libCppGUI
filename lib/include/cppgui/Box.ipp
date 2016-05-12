#include "./Box.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    void Bordered_box<Config, With_layout>::Aspect<Aspect_parent>::set_border(const Border &border)
    {
        _border = border;
    }

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    void Bordered_box<Config, With_layout>::Aspect<Aspect_parent>::draw_border(Canvas_t *canvas, const Point &offset)
    {
        auto b = _border.width;

        if (b > 0)
        {
            const auto &rect = p()->rectangle();

            auto ntvclr = Canvas_t::rgba_to_native( _border.color );

            auto x1 = rect.left(), y1 = rect.top(), x2 = rect.right(), y2 = rect.bottom();
            auto w = rect.ext.w, h = rect.ext.h;

            auto d = (Position_delta) b;

            fill_rect(canvas, offset + Point{ x1 + d, y1     }, { w - b,     b }, ntvclr); // top
            fill_rect(canvas, offset + Point{ x2 - d, y1 + d }, {     b, h - b }, ntvclr); // right
            fill_rect(canvas, offset + Point{ x1    , y2 - d }, { w - b,     b }, ntvclr); // bottom
            fill_rect(canvas, offset + Point{ x1    , y1     }, {     b, h - b }, ntvclr); // left
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Box__Layouter<Config, true>::Aspect<Aspect_parent>::set_padding(Width w)
    {
        set_padding({ w, w, w, w });
    }

    /* template<class Config, bool With_layout>
    template<class Aspect_parent>
    void Box__Layouter<Config, With_layout>::Aspect<Aspect_parent>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    } */

    template<class Config>
    template<class Aspect_parent>
    void Box__Layouter<Config, true>::Aspect<Aspect_parent>::set_padding(const std::array<Length, 4> &padding)
    {
        _padding = padding; // std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Config>
    template<class Aspect_parent>
    void Box__Layouter<Config, true>::Aspect<Aspect_parent>::add_padding(Rectangle &rect)
    {
        rect.pos.x += _padding[3] - 2;
        rect.pos.y += _padding[0] - 2;
        rect.ext.w -= _padding[3] + _padding[1] - 2 * 2;
        rect.ext.h -= _padding[0] + _padding[2] - 2 * 2;
    }

    template<class Config>
    template<class Aspect_parent>
    auto Box__Layouter<Config, true>::Aspect<Aspect_parent>::add_padding(const Rectangle &rect) -> Rectangle
    {
        Rectangle res { rect };

        add_padding( res );

        return res;
    }

    template<class Config>
    template<class Aspect_parent>
    auto Box__Layouter<Config, true>::Aspect<Aspect_parent>::add_padding(const Extents &ext) -> Extents
    {
        Extents res = ext;
        
        res.w += _padding[3] + _padding[1];
        res.h += _padding[0] + _padding[2];

        return res;
    }

    template<class Config>
    template<class Aspect_parent>
    void Box__Layouter<Config, true>::Aspect<Aspect_parent>::add_padding(Extents &ext)
    {
        ext.w += _padding[3] + _padding[1];
        ext.h += _padding[0] + _padding[2];
    }

} // ns cppgui