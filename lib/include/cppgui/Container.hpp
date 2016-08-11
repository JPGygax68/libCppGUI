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

#include "./Container_base.hpp"
#include "./Box_model.hpp"

namespace cppgui
{
    // Internals ----------------------------------------------------

    template<class Config>
    struct _container
    {
        // TODO: put into two-tiered form

        template<class Class, bool With_layout, class Parent> struct Layouter;

        template<class Class, class Parent> 
        struct Layouter<Class, false, Parent>: Parent {};

        template<class Class, class Parent> 
        struct Layouter<Class, true, Parent>: Parent
        {
            using Container_base_t = Container_base<Config, true>;

            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto compute_minimal_size() -> Extents;
            void layout_children(const Extents &);

        private:
            auto p() { return static_cast<Class*>(this); }

            std::unique_ptr<typename layouting<Config>::Manager> _manager;
        };
    };

    // Public class -------------------------------------------------

    template<class Class, class Config, bool With_layout, Box_model_definition BMDef, class Layouter>
    class Container: public 
        Layouter::template Aspect< Class,
            Box_model<Config, With_layout, BMDef>::template Aspect< Class,
                Container_base<Config, With_layout> > >
    {};

} // ns cppgui