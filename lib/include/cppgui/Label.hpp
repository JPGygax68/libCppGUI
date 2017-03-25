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
#include "./Boxed.hpp"


namespace cppgui {

    /** Label widget.
     */

    class Label: public Widget
    {
    public:

        void set_font(const Rasterized_font *);
        //auto font() const { return _font.source(); }
        void set_color(const RGBA &);
        void set_text(const std::u32string &);
        auto& text() const { return _text; }

        void init(Canvas *) override;

        void render(Canvas *, const Point &offset) override;

        // void change_text(const std::u32string &);

    protected:
        Font_resource           _font;
        std::u32string          _text;
        RGBA                    _color = {0, 0, 0, 1};

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:

        // Contract

        void init_layout() override;
        auto get_minimal_bounds() -> Bbox override;

        // Own methods

        void set_minor_alignment(Alignment align) { _minor_alignment = align; }
        void set_major_alignment(Alignment align) { _major_alignment = align; }
        // TODO: "change" versions of the above that update layout

    private:
        // "Stylesheet"
        //static constexpr auto default_padding() -> Padding { return { 4, 4, 4, 4 }; }

        Alignment               _minor_alignment = Alignment::cultural_minor_middle;
        Alignment               _major_alignment = Alignment::cultural_major_middle;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING

    };

} // ns cppgui
