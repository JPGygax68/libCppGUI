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

        if (_left && _left->widget)
        {
            _left->widget->set_position(rect.pos);
            _left->widget->set_extents ({ rect.ext.w * _left->size, rect.ext.h });
        }

        if (_right && _right->widget)
        {
            _right->widget->set_extents ({ rect.ext.w * _right->size, rect.ext.h });
            _right->widget->set_position({ rect.right() - _right->widget->extents().w, rect.pos.y });
        }

        // TODO: center (if any)

        Parent::layout();
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_line_layout<Config, true>::Aspect<Class, Parent>::add_element(Widget<Config, true> * widget)
    {
        _elements.push_back( std::make_unique<Element_ref>(widget) );
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_line_layout<Config, true>::Aspect<Class, Parent>::init_layout()
    {
        for (auto& elem: _elements)
        {
            p()->add_child(elem->_widget);
        }

        Parent::init_layout();
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_line_layout<Config, true>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        Extents total;

        for (auto& elem: _elements)
        {
            auto size = elem->_widget->get_minimal_size();

            total.w += std::max(size.w, elem->_min_length);
            total.h = std::max(total.h, size.h);
        }

        return total;
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_line_layout<Config, true>::Aspect<Class, Parent>::layout()
    {
        auto rect = p()->get_inner_rectangle();

        // TODO: PRELIMINARY IMPLEMENTATION, NOT ALLOCATING EXTRA HEIGHT YET

        for (auto& elem: _elements)
        {
            auto size = elem->_widget->get_minimal_size();

            elem->_widget->set_position(rect.pos);
            elem->_widget->set_extents ({ size.w, rect.ext.h });

            rect.pos.x += size.w; // TODO: spacing
        }

        Parent::layout();
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::add_element(Widget<Config, true> * widget)
    {
        _elements.push_back( std::make_unique<Element_ref>(widget) );
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::init_layout()
    {
        for (auto& elem: _elements)
        {
            p()->add_child(elem->_widget);
        }

        Parent::init_layout();
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_column_layout<Config, true>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        Extents total;

        for (auto& elem: _elements)
        {
            auto size = elem->_widget->get_minimal_size();

            total.w = std::max(total.w, size.w);
            total.h += std::max(size.h, elem->_min_length);
        }

        return total;
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::layout()
    {
        auto rect = p()->get_inner_rectangle();

        // TODO: PRELIMINARY IMPLEMENTATION, NOT ALLOCATING EXTRA HEIGHT YET

        for (auto& elem: _elements)
        {
            auto size = elem->_widget->get_minimal_size();

            elem->_widget->set_position(rect.pos);
            elem->_widget->set_extents ({ rect.ext.w, size.h });

            rect.pos.y += size.h; // TODO: spacing
        }

        Parent::layout();
    }

} // ns cppgui
