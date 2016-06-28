#include <cassert>

#include "./Container_layouter.hpp"

namespace cppgui
{
    template <class Config>
    template <class Class, class Parent>
    void Horizontal_box<Config, true>::Aspect<Class, Parent>::set_left(Widget<Config, true> *child, const Fraction<Length> &width)
    {
        assert(!_left);

        _left = std::make_unique<Element_ref>(child, width);
    }

    template <class Config>
    template <class Class, class Parent>
    void Horizontal_box<Config, true>::Aspect<Class, Parent>::set_center(Widget<Config, true> *child)
    {
        assert(!_left);

        _center = std::make_unique<Element_ref>(child);
    }

    template <class Config>
    template <class Class, class Parent>
    void Horizontal_box<Config, true>::Aspect<Class, Parent>::set_right(Widget<Config, true> *child, const Fraction<Length> & width)
    {
        assert(!_right);

        _right = std::make_unique<Element_ref>(child, width);
    }

    template <class Config>
    template <class Class, class Parent>
    void Horizontal_box<Config, true>::Aspect<Class, Parent>::init_layout()
    {
        if (_left   && _left  ->widget) p()->add_child( _left  ->widget );
        if (_center && _center->widget) p()->add_child( _center->widget );
        if (_right  && _right ->widget) p()->add_child( _right ->widget );

        Parent::init_layout();
    }

    template <class Config>
    template <class Class, class Parent>
    auto Horizontal_box<Config, true>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        Extents total;

        for (auto child: p()->children())
        {
            auto size = child->extents();
            total.w += size.w;
            total.h = std::min(total.h, size.h);
        }

        return total;
    }

    template <class Config>
    template <class Class, class Parent>
    void Horizontal_box<Config, true>::Aspect<Class, Parent>::layout()
    {
        auto rect = this->get_inner_rectangle();

        _left->widget->set_position(rect.pos);
        _left->widget->set_extents ({ rect.ext.w * _left->size, rect.ext.h });

        Parent::layout();
    }

} // ns cppgui
