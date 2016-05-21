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

    template<class Config, bool With_layout>
    inline void Container<Config, With_layout>::compute_view_from_data()
    {
    }

    // Layouter aspect ----------------------------------------------

    template <class Config, class Parent>
    inline void Container__Layouter<Config, true, Parent>::init_layout()
    {
    }

    template <class Config, class Parent>
    inline auto Container__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
    }

    template <class Config, class Parent>
    inline auto Container__Layouter<Config, true, Parent>::get_preferred_size() -> Extents
    {
    }

    template <class Config, class Parent>
    inline void Container__Layouter<Config, true, Parent>::layout()
    {
    }

    template <class Config, class Parent>
    inline void Container__Layouter<Config, true, Parent>::insert_child(Widget_t *child)
    {
    }

    template <class Config, class Parent>
    inline void Container__Layouter<Config, true, Parent>::drop_child(Widget_t *child)
    {
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout, class Parent>
    inline void Default__Container_base__Container_updater<Config, With_layout, Parent>::child_invalidated(Widget_t *)
    {
    }

} // ns cppgui