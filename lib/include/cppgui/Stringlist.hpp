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

#include <vector>

#include "./Box.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    #define CPPGUI_INSTANTIATE_STRINGLIST(Config, With_layout) \
        template cppgui::_stringlist<Config>; \
        _CPPGUI_INSTANTIATE_STRINGLIST(Config, With_layout)

    template<class Config>
    struct _stringlist {

        // Forward declarations

        template<bool With_layout, class Parent> struct Layouter;

        // Main class declaration ---------------------------------------

        #define _CPPGUI_INSTANTIATE_STRINGLIST_BASE(Class, Config, With_layout) \
            template cppgui::_stringlist<Config>::Base<Class, With_layout>; \
            template cppgui::_stringlist<Config>::Layouter<With_layout, \
                cppgui::Bordered_box<Config, With_layout, \
                    cppgui::Container<Config, With_layout>>>;

        template<class Class, bool With_layout>
        class Base: public 
            Layouter<With_layout, 
                Bordered_box<Config, With_layout,
                    Container<Config, With_layout> > >
        {
        public:
            using Widget_t = typename Widget<Config, With_layout>;
            using Canvas_t = typename Widget_t::Canvas_t;
            using Container_t = Container<Config, With_layout>;

            Base();

            void set_font(const Rasterized_font *font);

            void init() override;

            void compute_view_from_data() override;

            void add_item(const std::string &text);

            void add_item(const std::u32string &text);

            void render(Canvas_t *canvas, const Point &offset) override;

        protected:
            using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;
            using Font_resource = typename Widget_t::Font_resource;

            void compute_content_rectangle();
            auto item_height() const;

            Vertical_scrollbar_t        _vert_sbar;
            Separator                   _sbar_separator = { 1, {0.2f, 0.2f, 0.2f, 1} }; // TODO: get color from static method or stylesheet
            Padding                     _item_padding = { 2, 3, 2, 3 };
            Separator                   _item_separator = { 1, {0.2f, 0.2f, 0.2f, 1} }; // TODO: get from method / stylesheet

            Font_resource               _font;
            std::vector<std::u32string> _items;

            Position                    _vert_sep_pos;
            Position                    _ascent, _descent;

            Rectangle                   _content_rect;
        };

        // Layouter aspect ------------------------------------------

        /** Dummy template specialization for when With_layout = false.
        */
        template<class Parent> struct Layouter<false, Parent>: public Parent {};

        /** "Real" layouter specialization that will be selected when With_layout = true.
        */
        template<class Parent>
        struct Layouter<true, Parent>: public Parent
        {
            void init_layout() override;

            auto get_minimal_size() -> Extents override;

            void layout() override;

        protected:
            class Stringlist_t: public Base<Config, true> { friend struct Layouter; };

            auto p() { return static_cast<Stringlist_t *>(this); }

            void compute_text_extents();
        };

    }; // pseudo-ns _stringlist

    #define _CPPGUI_INSTANTIATE_STRINGLIST(Config, With_layout) \
        template cppgui::Stringlist<Config, With_layout>; \
        _CPPGUI_INSTANTIATE_STRINGLIST_BASE(cppgui::Stringlist<Config, With_layout>, Config, With_layout)
        
    template<class Config, bool With_layout>
    class Stringlist: public _stringlist<Config>::template Base<Stringlist<Config, With_layout>, With_layout> { };

} // ns cppgui