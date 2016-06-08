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

#include "./Container.hpp"

namespace cppgui
{
    #define CPPGUI_INSTANTIATE_LAYOUTING(Config) \
        template cppgui::layouting<Config>;

    template<class Config>    
    struct layouting
    {        
        using Widget_t = Widget<Config, true>;
        using Container_t = Container<Config, true>;

        class Manager
        {
        public:

            virtual ~Manager() = default;

            virtual auto get_minimal_size(Container_t &) -> Extents = 0;
            virtual void layout(Container_t &) = 0;

            void set_padding(const Padding &);
            void set_padding(Padding &&);
            void set_spacing(Length spacing); // TODO: move to a more specialized descendant ?

        protected:

            auto minimal_size_horizontal(Container_t &) -> Extents;

            Padding         _padding;
            Length          _spacing = 0; 
        };

        class Header_content: public Manager
        {
        public:
            auto get_minimal_size(Container_t &) -> Extents override;
            void layout(Container_t &) override;
        };

        class Content_footer: public Manager
        {
        public:
            auto get_minimal_size(Container_t &) -> Extents override;
            void layout(Container_t &) override;
        };

        class Content_tail: public Manager
        {
        public:
            auto get_minimal_size(Container_t &) -> Extents override;
            void layout(Container_t &) override;
        };

        class Stack: public Manager
        {
        public:
            auto get_minimal_size(Container_t &) -> Extents override;
            void layout(Container_t &) override;
        };

        /** Left_to_right will do exactly what the name says: it will put the elements one after 
            the other, from left to right.
            Note that it does NOT, at this time, attempt to make use of available extra space,
            meaning extra space to the right will remain empty unused.
         */
        class Left_to_right: public Manager
        {
        public:
            auto get_minimal_size(Container_t &) -> Extents override;
            void layout(Container_t &) override;
        };

    }; // templated ns layout

} // ns cppgui
