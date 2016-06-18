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
#include "./Container_base.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config>
    struct _stringlist {

        // Forward declarations

        template<class Class, bool With_layout, class Parent> struct Layouter;

        // Default settings

        static constexpr auto default_border_width = 1; // TODO: base on DPI

        // Main class declaration ---------------------------------------

        //#define _CPPGUI_INSTANTIATE_STRINGLIST_BASE(Config, With_layout, /*BoxModel*/...) \
        //    template cppgui::_stringlist<Config>::Base<With_layout, __VA_ARGS__>; \
        //    template cppgui::_stringlist<Config>::Layouter<With_layout, __VA_ARGS__, \
        //        cppgui::Box<Config, With_layout, \
        //            __VA_ARGS__< \
        //                cppgui::Container_base<Config, With_layout> > > >;

        template<bool With_layout, int BorderWidth>
        class Base: public 
            Layouter<Base<With_layout, BorderWidth>, With_layout, 
                Box<Config, With_layout, 
                    Fixed_border_and_padding_box_model<BorderWidth, 0,
                        Container_base<Config, With_layout> > > >
        {
        public:
            using Widget_t = Widget<Config, With_layout>;
            using Parent_t = Container_base<Config, With_layout>;
            using Canvas_t = typename Widget_t::Canvas_t;
            using Container_base_t = Container_base<Config, With_layout>;
            using Keyboard = typename Config::Keyboard;
            using Keycode = typename Keyboard::Keycode;

            using Item_selected_handler  = std::function<void(Index, const std::u32string &)>;
            using Item_activated_handler = std::function<void(Index, const std::u32string &)>;

            Base();

            void on_item_selected (Item_selected_handler );
            void on_item_activated(Item_activated_handler);

            void set_font(const Rasterized_font *font);

            void init() override;
            void compute_view_from_data() override;

            void add_item(const std::string &text);
            void add_item(const std::u32string &text);

            void render(Canvas_t *canvas, const Point &offset) override;

            void mouse_motion(const Point &) override;
            void mouse_button(const Point &, int button, Key_state, Count clicks) override;
            void mouse_wheel(const Vector &) override;
            void mouse_exit() override;
            void key_down(const Keycode &) override;

        protected:
            using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;
            using Font_resource = typename Widget_t::Font_resource;
            using Native_color = typename Canvas_t::Native_color;

            // Actions

            void select_next();
            void select_previous();
            void page_down();
            void page_up();
            void activate_selected_item();

            // Internal methods

            void compute_content_rectangle();
            auto item_height() const;
            auto fully_visible_item_count() const -> Count;
            void move_by_elements(int delta);   
            void move_by_pages(int delta);
            void move_by_fraction(const Fraction<int>& delta);
            void update_scrollbar_position();
            void raise_item_activated(Index index, const std::u32string & item);
            auto item_at_pos(const Point &pos) -> Index;
            void select_item(Index);

            Vertical_scrollbar_t        _vert_sbar;
            Separator                   _sbar_separator = this->interior_separator();
            Padding                     _item_padding = { 2, 3, 2, 3 };                 // TODO: get from static method or stylesheet
            Separator                   _item_separator = this->grid_separator();

            Item_selected_handler       _on_item_selected;
            Item_activated_handler      _on_item_activated;

            Font_resource               _font;
            std::vector<std::u32string> _items;

            Position                    _vert_sep_pos;
            Position                    _ascent, _descent;

            Rectangle                   _content_rect;

            Index                       _first_vis_item = 0;
            Index                       _selected_item = -1;
            Index                       _hovered_item = -1;
        };

        // Layouter aspect ------------------------------------------

        /** Dummy template specialization for when With_layout = false.
        */
        template<class Class, class Parent> struct Layouter<Class, false, Parent>: public Parent {};

        /** "Real" layouter specialization that will be selected when With_layout = true.
        */
        template<class Class, class Parent>
        struct Layouter<Class, true, Parent>: Parent
        {
            void init_layout() override;

            auto get_minimal_size() -> Extents override;

            void layout() override;

        protected:
            class Stringlist_t: public Class  { friend struct Layouter; };

            auto p() { return static_cast<Stringlist_t *>(this); }

            void compute_text_extents();
        };

    }; // pseudo-ns _stringlist

    // Specializations ----------------------------------------------

    //#define CPPGUI_INSTANTIATE_STRINGLIST(Config, With_layout, /*BoxModel*/...) \
    //    template cppgui::Stringlist<Config, With_layout, __VA_ARGS__>; \
    //    template cppgui::_stringlist<Config>; \
    //    _CPPGUI_INSTANTIATE_STRINGLIST_BASE(Config, With_layout, __VA_ARGS__)

    template<class Config, bool With_layout, int BorderWidth = _stringlist<Config>::default_border_width>
    class Stringlist: public _stringlist<Config>::template Base<With_layout, BorderWidth> { };

} // ns cppgui
