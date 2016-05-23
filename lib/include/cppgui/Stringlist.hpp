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
#include <string>

#include "./unicode.hpp"
#include "./Resource.hpp"
#include "./Box.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    template<class Config>
    struct _stringlist {

        // Forward declarations

        template<bool With_layout, class Parent> struct Layouter;

        // Main class declaration ---------------------------------------

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

            Base()
            {
                add_child( &_vert_sbar );

                _vert_sbar.on_navigation([this](Navigation_unit unit, const Fraction<int> &delta) {

                    switch(unit)
                    {
                    case Navigation_unit::element:
                        // TODO
                        break;
                    case Navigation_unit::page:
                        // TODO
                        break;
                    case Navigation_unit::fraction:
                        // TODO
                        break;
                    }
                    
                    //static_cast<Pane*>(_content)->scroll(unit, delta);
                });
            }

            void set_font(const Rasterized_font *font)
            {
                _font.assign( font );
            }

            void init() override
            {
                /** This is where the widget establishes its connection with the backends
                (available via root_widget()->canvas() etc. ).
                */

                _font.translate( root_widget()->canvas() );

                Container_t::init();
            }

            void compute_view_from_data() override
            {
                /** This is another entry point that gets called recursively upon
                initialization. Its name says what it is intended for: to 
                set internal state so as to reflect the bound data.
                */

                compute_content_rectangle();

                auto visible_items = (_content_rect.height() + _item_separator.width) / (item_height() + _item_separator.width);

                _vert_sbar.define_values(_items.size(), visible_items);

                Container_t::compute_view_from_data();
            }

            void add_item(const std::string &text)
            {
                add_item( utf8_to_utf32(text) );
            }

            void add_item(const std::u32string &text)
            {
                _items.push_back( text );
            }

            void render(Canvas_t *canvas, const Point &offset) override
            {
                Point pos = offset + position();

                // fill(canvas, offset, Canvas_t::rgba_to_native({1, 1, 1, 1}) );

                draw_border(canvas, offset);
                
                // Render the separator between the scrollbar and the content rectangle
                draw_vert_separator(canvas, offset, _sbar_separator, _vert_sep_pos);

                // Render the scrollbar (delegate to the container)
                Container_t::render(canvas, offset);

                // Render the items
                // TODO: clip
                
                // Item rectangle
                Rectangle r_item { _content_rect };
                r_item.ext.h = item_height();
                
                // Separator rectangle
                Rectangle r_sep { r_item };
                r_sep.pos.y += (Position_delta) r_item.ext.h;
                r_sep.ext.h = _item_separator.width;

                // Draw all items
                for (auto i = 0U; ; )
                {
                    // Draw item background
                    fill_rect(canvas, r_item, pos, Canvas_t::rgba_to_native({ 1, 1, 1, 1 }));

                    // Render item text
                    Point p_text = r_item.pos + Extents{ _item_padding[3], _item_padding[0] + _ascent };
                    Width w_text = r_item.width() - _item_padding[3] - _item_padding[1];
                    canvas->render_text( _font.get(), pos.x + p_text.x, pos.y + p_text.y, _items[i].data(), _items[i].size(), w_text);

                    // Done ?
                    if (++i >= _items.size()) break;

                    r_item.pos.y += (Position_delta) r_item.ext.h;
                    if (r_item.pos.y >= _content_rect.bottom()) break;

                    // Draw item separator
                    r_sep.pos.y = r_item.pos.y;
                    fill_rect(canvas, r_sep, pos, Canvas_t::rgba_to_native(_item_separator.color) );

                    r_item.pos.y += _item_separator.width;
                    if (r_item.pos.y >= _content_rect.bottom()) break;
                }
            }

        protected:
            using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;
            using Font_resource = typename Widget_t::Font_resource;

            void compute_content_rectangle()
            {
                Length sb_width = _vert_sbar.extents().w + _sbar_separator.width;

                _content_rect = Rectangle{ extents() } - Margins{ _border.width, _border.width + sb_width, _border.width, _border.width };
            }
            auto item_height() const
            {
                return (Length) (_item_padding[0] + _ascent - _descent + _item_padding[2]);
            }

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
            void init_layout() override
            {
                /** The init_layout() method is called on the complete widget tree 
                before either get_minimal_size() or layout(). It is intended as 
                an occasion to compute measurements (typically of text strings) 
                that can then be used in both get_minimal_size() and layout().
                */

                compute_text_extents();

                Parent::init_layout();
            }

            auto get_minimal_size() -> Extents override
            {
                /** The get_minimal_size() method is intended to be called recursively
                by containers, or from a container's layout() method to help it
                decide how to place and size its children.
                */

                return {};
            }

            void layout() override
            {
                // Preparations
                auto exts = p()->extents();
                auto vertsb_minsz = p()->_vert_sbar.get_minimal_size();

                // Vertical scrollbar
                p()->_vert_sep_pos = exts.w - p()->_border.width - vertsb_minsz.w - p()->_sbar_separator.width;
                p()->_vert_sbar.set_position({ exts.right() - (Position_delta) (vertsb_minsz.w + p()->_border.width), (Position) p()->_border.width });
                p()->_vert_sbar.set_extents ({ vertsb_minsz.w, exts.h - 2 * p()->_border.width });
                p()->_vert_sbar.layout();

                Parent::layout();
            }

        protected:
            class Stringlist_t: public Base<Config, true> { friend struct Layouter; };

            auto p() { return static_cast<Stringlist_t *>(this); }

            void compute_text_extents()
            {
                if (p()->_font.source())
                {
                    // TODO: support other cultures
                    auto bbox = p()->_font.source()->compute_text_extents(0, U"My", 2);
                    p()->_ascent  = bbox.y_max;
                    p()->_descent = bbox.y_min;
                    //p()->_mean_char_width = (bbox.width() + 1) / 2;
                }
            }
        };

    }; // pseudo-ns _stringlist

    template<class Config, bool With_layout>
    class Stringlist: public _stringlist<Config>::Base<Stringlist<Config, With_layout>, With_layout> {};

} // ns cppgui
