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

#include "./utils.hpp" // TODO: move to Widget.hpp

#include "./Widget.hpp"
#include "./Box_model.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout>
    struct Label__Layouter
    {
        template<class Class, class Parent> 
        struct Aspect: Parent {};
    };

    /** Label widget.
     */

    template <class Config, bool With_layout, Box_model_definition BMDef>
    class Label: public 
        Label__Layouter<Config, With_layout>::template Aspect<Label<Config, With_layout, BMDef>,
        Box_model<Config, With_layout, BMDef>::template Aspect<Label<Config, With_layout, BMDef>,
        Widget<Config, With_layout> > >
    {
    public:
        using Renderer = typename Config::Renderer;
        //using Font_handle = typename Renderer::font_handle;
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_resource = typename Widget_t::Font_resource;

        void set_font(const Rasterized_font *);
        auto font() const { return _font.source(); }
        void set_text(const std::u32string &);
        auto& text() const { return _text; }

        void init() override;

        void render(Canvas_t *, const Point &offset) override;

        // void change_text(const std::u32string &);

    protected:
        Font_resource           _font;
        std::u32string          _text;
        Point                   _text_origin; // origin of first character of label
        Rectangle               _text_rect;
    };

    class Single_element_layout;

    // Layouter aspect ----------------------------------------------

    template <class Config>
    struct Label__Layouter<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            // Layouter aspect contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Own methods

            void set_minor_alignment(Alignment align) { _minor_alignment = align; }
            void set_major_alignment(Alignment align) { _major_alignment = align; }
            // TODO: "change" versions of the above that update layout

        private:
            class Implementation: public Class { friend struct Aspect; };
            auto p() { return static_cast<Implementation*>(this); }

            // "Stylesheet"
            //static constexpr auto default_padding() -> Padding { return { 4, 4, 4, 4 }; }

            Alignment               _minor_alignment = Alignment::cultural_minor_middle;
            Alignment               _major_alignment = Alignment::cultural_major_middle;
            Text_bounding_box       _bounding_box;
            //Single_element_layout   _layout;
        };
    };

} // ns cppgui
