#pragma once 

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <cassert>
#include <numeric>

#include "./oriented_geometry.hpp"

#include "./Container_layouter.hpp"

namespace cppgui
{
    template <class Main, class Parent>
    template <class Layouter>
    void Delegating_layouter::Aspect<Main, Parent>::create_layouter()
    {
        //auto ptr = new typename Layouter::template Aspect<Main, Parent>{};
        //_layouter.reset( static_cast<Detached_layouter_base_t*>(ptr) );
        _layouter.reset( new typename Layouter::template Aspect<Main, Parent>{} );

        _layouter->set_main( static_cast<Main*>(this) );
    }

    template <class Class, class ElementRef, class Config, class Parent, Aspect_injection Injection>
    void Container_layouter_base<Class, ElementRef, Config, Parent, Injection>::_add_element(ElementRef *elem)
    {
        _elements.push_back( std::unique_ptr<ElementRef>{ elem } );
    }

    template <class Class, class ElementRef, class Config, class Parent, Aspect_injection Injection>
    auto Container_layouter_base<Class, ElementRef, Config, Parent, Injection>::sum_of_weights() -> float
    {
        return std::accumulate(std::begin(_elements), std::end(_elements), 0.0f, [](float sum, auto& elem) { return sum + elem->weight; });
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    void Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::set_left(Widget<Config, true> *child, const Fraction<Length> &width)
    {
        assert(!_left);

        _left = std::make_unique<Element_ref>(child, width);
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    void Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::set_center(Widget<Config, true> *child)
    {
        assert(!_center);

        _center = std::make_unique<Element_ref>(child);
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    void Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::set_right(Widget<Config, true> *child, const Fraction<Length> & width)
    {
        assert(!_right);

        _right = std::make_unique<Element_ref>(child, width);
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    void Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::init_layout()
    {
        if (_left   && _left  ->widget) p()->add_child( _left  ->widget );
        if (_center && _center->widget) p()->add_child( _center->widget );
        if (_right  && _right ->widget) p()->add_child( _right ->widget );

        Parent_t::init_layout();
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    auto Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        Extents total;

        for (auto child: this->p()->children())
        {
            auto size = child->extents();
            total.w += size.w;
            total.h = std::min(total.h, size.h);
        }

        return total;
    }

    template <class Config, Aspect_injection Injection>
    template <class Class, class Parent>
    void Horizontal_box<Config, true, Injection>::Aspect<Class, Parent>::layout()
    {
        auto rect = this->p()->content_rectangle(); // this->get_inner_rectangle();

        Position x1 = 0, x2 = rect.ext.w;

        if (_left && _left->widget)
        {
            _left->widget->set_position(rect.pos);
            auto w = rect.ext.w * _left->size;
            _left->widget->set_extents ({ w, rect.ext.h });
            x1 += w;
        }

        if (_right && _right->widget)
        {
            auto w = rect.ext.w * _right->size;
            _right->widget->set_extents ({ w, rect.ext.h });
            _right->widget->set_position({ rect.right() - w, rect.pos.y });
            x2 -= w;
        }

        if (_center && _center->widget)
        {
            _center->widget->set_position({ x1, 0 });
            _center->widget->set_extents ({ x2 - x1, rect.ext.h });
        }

        Parent_t::layout();
    }

    // Single_beam_flow_layout ------------------------------------------------

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    void Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::set_spacing(Length spacing)
    {
        _spacing = spacing;
    }

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    auto Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::add_element(Widget<Config, true> * widget, float weight) 
        -> Class &
    {
        this->_add_element( new Element_ref{widget, weight} );

        return * p();
    }

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    auto Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::add_spacer(float weight) -> Class &
    {
        assert(this->elements().empty() || this->elements().back()->widget); // predecessor must not be spacer

        this->_add_element( new Element_ref{ nullptr, weight } );

        return * p();
    }

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    void Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::init_layout()
    {
        for (auto& elem: this->elements())
        {
            if (elem->widget) p()->add_child(elem->widget);
        }

        Parent::init_layout();
    }

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    auto Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::get_minimal_size() -> Extents
    {
        using Oriented_extents_t = Oriented_extents<Orientation, Default_latitudinal_orientation<Orientation>::value, Parent::y_axis_up>;
        
        Oriented_extents_t total;

        Widget_t *prev_widget = nullptr;

        for (auto i = 0U; i < this->elements().size(); ++i)
        {
            auto& elem = this->elements()[i];

            // Widget element ?
            if (elem->widget && elem->widget->visible())
            {
                if (prev_widget) total.length() += _spacing;

                Oriented_extents_t size{ elem->widget->get_minimal_size() };
                total.breadth() = std::max(total.breadth(), size.breadth());
                total.length () += std::max( static_cast<Length>(size.length()), elem->_min_length );

                prev_widget = elem->widget;
            }
            else // Not a widget, just a spacer
            {
                // Does not occupy any space on its own

                prev_widget = nullptr;
            }
        }

        return this->add_boxing( total );
    }

    template <class Config, Orientation Orientation, Aspect_injection Injection>
    template <class Class, class Parent>
    void Single_beam_flow_layout<Config, true, Orientation, Injection>::Aspect<Class, Parent>::layout()
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

        using Oriented_rectangle_t = Oriented_rectangle<Orientation, Default_latitudinal_orientation<Orientation>::value, Parent::y_axis_up>;
        using Oriented_extents_t   = Oriented_extents  <Orientation, Default_latitudinal_orientation<Orientation>::value, Parent::y_axis_up>;

        Oriented_rectangle_t rect{ this->p()->content_rectangle() };

        // Total of all "weights"
        auto total_weight = this->sum_of_weights();
        //std::accumulate(std::begin(_elements), std::end(_elements), 0.0f, [](float sum, auto& elem) { return sum + elem->weight; });

        // Calculate extra height to distribute among
        Oriented_extents_t realsz{ this->p()->extents() }, minsz{ get_minimal_size() };
        auto extra_length = realsz.length() - minsz.length();

        // Assign position and size to all widgets

        Widget_t *prev_widget = nullptr;
        Directed_position< Axis_reversed<Orientation, Parent::y_axis_up>::value > lon = rect.longitude();

        for (auto i = 0U; i < this->elements().size(); ++i)
        {
            auto& elem = this->elements()[i];

            auto extra_portion = static_cast<Length>( total_weight > 0 ? extra_length * elem->weight / total_weight : 0 );

            if (elem->widget && elem->widget->visible())
            {
                if (prev_widget) lon += _spacing;

                Oriented_extents_t size{ elem->widget->get_minimal_size() };

                auto l = size.length() + extra_portion;

                Oriented_rectangle_t r;
                r.set_longitudinal_segment( lon, l );
                r.set_latitudinal_segment( rect.latitude(), rect.extents().breadth() );

                elem->widget->set_rectangle( r );

                lon += l;

                prev_widget = elem->widget;
            }
            else
            {
                lon += extra_portion;

                prev_widget = nullptr;
            }
        }

        Parent_t::layout();
    }

} // ns cppgui
