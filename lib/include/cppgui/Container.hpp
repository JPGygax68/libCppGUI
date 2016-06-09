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

#include <memory>

#include "./Widget.hpp"
#include "./Box.hpp"
#include "./Container_base.hpp"
#include "./layout_managers.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    enum class Layout_type 
    { 
        none, 
        header_content, 
        content_footer, 
        head_content,           // horizontal: fixed-width head, followed by stretching content TODO: better name! 
        content_tail,           // opposite of head_content
        stack,
        end
    };

    // Forward-declarations

    template <class Config, bool With_layout, class Parent> struct Container__Layouter;

    // Main class

    #define CPPGUI_INSTANTIATE_CONTAINER(Config, With_layout) \
        template cppgui::Container<Config, With_layout>; \
        _CPPGUI_INSTANTIATE_CONTAINER_LAYOUTER(Config, With_layout, cppgui::Container_base<Config, With_layout>)

    /** This class adds layouting capabilities to Container_base<>.
        These capabilities are of course seated in the Layouter aspect, and are derived
        from the Layouter aspect of Box<>, which defines borders and padding.
        TODO: add parameters to Container<> so that it will support forms of layouting that 
            do not use borders and padding ? -> those parameters would have to be propagated
            to the layout managers, too
     */
    template <class Config, bool With_layout>
    class Container: 
        public Container__Layouter<Config, With_layout,
            Container_base<Config, With_layout> >
    {
    public:
    };

    // Layouter aspect

    #define _CPPGUI_INSTANTIATE_CONTAINER_LAYOUTER(Config, With_layout, ...) \
        template cppgui::Container__Layouter<Config, With_layout, __VA_ARGS__>; \
        template cppgui::Box__Layouter<Config, With_layout, __VA_ARGS__>;

    template <class Config, class Parent>
    struct Container__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent >
    {
        using Widget_t = typename Widget<Config, true>;
        using Container_base_t = Container_base<Config, true>;
        class Container_t: public Container<Config, true> { friend struct Container__Layouter; };

        // Layouter contract

        auto get_minimal_size() -> Extents override;
        void layout() override;

        // Specific interface

        //void set_layout_type(Layout_type type ) { _layout_type = type; }
        template<class ManagerType> void set_layout_manager()
        {
            _manager.reset( new ManagerType{} ); 
            _manager->set_padding(this->_padding); // TODO: should padding really be a member of Container ?
        };
        auto layout_manager() { return _manager.get(); }

        void insert_child(Widget_t *); // TODO: find a better name OR support insertion index
        void drop_child(Widget_t *);

    private:
        auto p() { return static_cast<Container_t*>(static_cast<Container<Config, true>*>(this)); }

        std::unique_ptr<typename layouting<Config>::Manager> _manager;
    };

    // Nil implementation of the Layouter aspect

    template <class Config, class Parent>
    struct Container__Layouter<Config, false, Parent> 
    {
        // void recalc_sizes() {}
    };
        
} // ns cppgui
