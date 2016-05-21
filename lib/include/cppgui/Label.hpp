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

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout, class Parent> struct Label__Layouter;

    template <class Config, bool With_layout> class Root_widget;

    /** Label, without layouting.
     */
    template <class Config, bool With_layout>
    class Label: public Label__Layouter<Config, With_layout, Widget<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        //using Font_handle = typename Renderer::font_handle;
        using Root_widget_t = Root_widget<Config, With_layout>;
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_resource = typename Widget_t::Font_resource;

        void set_font(const Rasterized_font *font)
        {
            _font.assign(font);
        }
        auto font() const { return _font.source(); }
        void set_text(const std::u32string &text)
        {
            _text = text;
        }
        auto text() const { return _text; }

        void init() override
        {
            Widget_t::init();

            _font.translate( root_widget()->canvas() );
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            fill(canvas, offset, rgba_to_native(background_color())); 

            auto pos = offset + position();
            canvas->render_text(_font.get(), pos.x + _text_origin.x, pos.y + _text_origin.y, _text.data(), _text.size());

            if (has_focus())
            {
                auto r = _text_rect + Extents{3, 2};
                canvas->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
            }
        }

        // void change_text(const std::u32string &);

    protected:
        Font_resource           _font;
        std::u32string          _text;
        Point                   _text_origin; // origin of first character of label
        Rectangle               _text_rect;
    };

    class Single_element_layout;

    template <class Config, class Parent>
    struct Label__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent>
    {
        Label__Layouter()
        {
            set_padding( default_padding() );
        }

        // Layouter aspect contract

        void init_layout() override
        {
            _layout.set_major_alignment(_major_alignment);
            _layout.set_minor_alignment(_minor_alignment);
            _layout.set_text_element(p()->font(), p()->_text.data(), p()->_text.size(), & p()->_text_origin, & p()->_text_rect);
        }
        auto get_minimal_size() -> Extents override
        {
            assert(!p()->text().empty()); // TODO: TENTATIVE RULE: layouting may not occur before conditions are met (font, text must be set) ?

            return _layout.compute_minimal_size(_padding);
        }
        void layout() override
        {
            _layout.compute_layout( p()->extents(), _padding);
        }

        // Own methods

        void set_minor_alignment(Alignment align) { _minor_alignment = align; }
        void set_major_alignment(Alignment align) { _major_alignment = align; }
        // TODO: "change" versions of the above that update layout

    private:
        class Label_t: public Label<Config, true> { friend struct Label__Layouter; };
        auto p() { return static_cast<Label_t*>(static_cast<Label<Config, true>*>(this)); }

        // "Stylesheet"
        static constexpr auto default_padding() -> Padding { return { 4, 4, 4, 4 }; }

        Alignment               _minor_alignment = Alignment::cultural_minor_middle;
        Alignment               _major_alignment = Alignment::cultural_major_middle;
        Single_element_layout   _layout;
    };

} // ns cppgui
