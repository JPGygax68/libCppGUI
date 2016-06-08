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

#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    // Layouter aspect ----------------------------------------------

    template <class Config, class Parent>
    auto Container__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        assert( _manager );

        return _manager->get_minimal_size( *p() );
    }

    template <class Config, class Parent>
    void Container__Layouter<Config, true, Parent>::layout()
    {
        assert( _manager );

        _manager->layout( *p() );

        Container_base_t::layout();
    }

    template <class Config, class Parent>
    void Container__Layouter<Config, true, Parent>::insert_child(Widget_t *child)
    {
        p()->add_child(child);
        layout();
    }

    template <class Config, class Parent>
    void Container__Layouter<Config, true, Parent>::drop_child(Widget_t *child)
    {
        if (this->contains_widget( this->root_widget()->mouse_holder() ))
        {
            this->root_widget()->release_mouse();
        }

        p()->remove_child(child);

        layout();
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout, class Parent>
    void Default__Container_base__Container_updater<Config, With_layout, Parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui