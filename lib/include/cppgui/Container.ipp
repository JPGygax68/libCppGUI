#pragma once

#include "./Container.hpp"

#include "./Container_base.ipp"
#include "./Box_model.ipp"

namespace cppgui
{
 
    template <class Config>
    template <class Class, class Parent>
    auto _container<Config>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        return compute_minimal_size();
    }

    template <class Config>
    template <class Class, class Parent>
    void _container<Config>::Layouter<Class, true, Parent>::layout()
    {
        _manager->layout( *this, p()->extents() );

        Container_base_t::layout(); // will propagate layout() to the children
    }

    template <class Config>
    template <class Class, class Parent>
    auto _container<Config>::Layouter<Class, true, Parent>::compute_minimal_size() -> Extents
    {
        assert( _manager );

        return _manager->get_minimal_size( *p());
    }

    template <class Config>
    template <class Class, class Parent>
    void _container<Config>::Layouter<Class, true, Parent>::layout_children(const Extents &ext) // TODO: rect!
    {
        assert( _manager );

        _manager->layout( *p(), ext);

        Container_base_t::layout();
    }

} // ns cppgui