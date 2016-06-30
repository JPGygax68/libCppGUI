#include <cassert>
#include <numeric>

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
    void Single_line_layout<Config, true>::Aspect<Class, Parent>::set_spacing(Width spacing)
    {
        _spacing = spacing;
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_line_layout<Config, true>::Aspect<Class, Parent>::add_element(Widget<Config, true> *widget, float weight) -> Class &
    {
        _elements.push_back( std::make_unique<Element_ref>(widget, weight) );

        return * p();
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

        total.w += (_elements.size() - 1) * _spacing;

        return total;
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_line_layout<Config, true>::Aspect<Class, Parent>::layout()
    {
        auto rect = p()->get_inner_rectangle();

        // Total of all "weights"
        auto total_weight = std::accumulate(std::begin(_elements), std::end(_elements), 0.0f, [](float sum, auto& elem) { return sum + elem->_weight; });

        // Calculate extra height to distribute among
        auto extra_width = rect.ext.w - get_minimal_size().w;

        for (auto& elem: _elements)
        {
            auto size = elem->_widget->get_minimal_size();

            Length w = size.w + (total_weight > 0 ? static_cast<Length>( extra_width * elem->_weight / total_weight ) : 0);

            elem->_widget->set_position(rect.pos);
            elem->_widget->set_extents ({ w, rect.ext.h });

            rect.pos.x += w + _spacing; 
        }

        Parent::layout();
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::set_spacing(Length spacing)
    {
        _spacing = spacing;
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_column_layout<Config, true>::Aspect<Class, Parent>::add_element(Widget<Config, true> * widget, float weight) -> Class &
    {
        _elements.push_back( std::make_unique<Element_ref>(widget, weight) );

        return *p();
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_column_layout<Config, true>::Aspect<Class, Parent>::add_spacer(float weight) -> Class &
    {
        assert(_elements.empty() || _elements.back()->_widget); // predecessor must not be spacer

        _elements.push_back( std::make_unique<Element_ref>(nullptr, weight) );

        return *p();
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::init_layout()
    {
        for (auto& elem: _elements)
        {
            if (elem->_widget) p()->add_child(elem->_widget);
        }

        Parent::init_layout();
    }

    template <class Config>
    template <class Class, class Parent>
    auto Single_column_layout<Config, true>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        Extents total;

        for (auto i = 0U; i < _elements.size(); ++i)
        {
            auto& elem =_elements[i];

            // Widget element ?
            if (elem->_widget)
            {
                if (i > 0 && _elements[i-1]->_widget) total.h += _spacing;

                auto size = elem->_widget->get_minimal_size();
                total.w = std::max(total.w, size.w);
                total.h += std::max(size.h, elem->_min_length);
            }
            else // Not a widget, just a spacer
            {
                // Does not occupy any space on its own
            }
        }

        return total;
    }

    template <class Config>
    template <class Class, class Parent>
    void Single_column_layout<Config, true>::Aspect<Class, Parent>::layout()
    {
        /** Note: the algorithm used here gives each element its minimum height, plus a portion of
                any available extra height, attributed according to its relative "weight".
                It follows from this that this layouter is not suited for allocating available space
                in precisely defined portions.

            Note 2: a spacer is treated differently from a widget in that the default spacing will
                not be added either before or after a spacer - instead, the spacer replaces the
                default spacing.
                This is another reason why this layouter is not suited for precise positioning.
         */

        auto rect = p()->get_inner_rectangle();

        // Total of all "weights"
        auto total_weight = std::accumulate(std::begin(_elements), std::end(_elements), 0.0f, [](float sum, auto& elem) { return sum + elem->_weight; });

        // Calculate extra height to distribute among
        auto extra_height = rect.ext.h - get_minimal_size().h;

        // Assign position and size to all widgets
        for (auto i = 0U; i < _elements.size(); ++i)
        {
            auto& elem = _elements[i];

            auto h_extra = static_cast<Length>( extra_height * elem->_weight / total_weight );

            if (elem->_widget)
            {
                if (i > 0 && _elements[i-1]->_widget) rect.pos.y += _spacing;

                auto size = elem->_widget->get_minimal_size();

                Length h = size.h + h_extra;

                elem->_widget->set_position(rect.pos);
                elem->_widget->set_extents ({ rect.ext.w, h });

                rect.pos.y += h;
            }
            else
            {
                rect.pos.y += h_extra;
            }
        }

        Parent::layout();
    }

} // ns cppgui
