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

#include "./Widget.hpp"
#include "./Box.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Pane, class Parent> struct Scrollbox__Layouter;
    template<class Config, bool With_layout> class Scrollable_pane;

    // Class definition

    // TODO: rename to Scrollbox_base ?

    template<class Config, bool With_layout, class Pane> // = Scrollable_pane<Config, With_layout>>
    class Scrollbox: 
        public Scrollbox__Layouter<Config, With_layout, Pane,
            Bordered_box<Config, With_layout, Container<Config, With_layout> > >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Parent_t = Container_t;
        using Canvas_t = typename Canvas<typename Config::Renderer>;
        using Scrollable_pane_t = Pane; // Scrollable_pane<Config, With_layout>;
        using Keyboard = typename Config::Keyboard;
        using Keycode = typename Keyboard::Keycode;
        //using List_pane_t = List_pane_base<Config, With_layout>;

        using Navigation_handler = std::function<void(Navigation_unit)>;

        Scrollbox();

        void init() override;

        void compute_view_from_data() override;

        void on_navigation(Navigation_handler);

        void set_content_pane(Scrollable_pane_t *);
        auto content_pane() { return _content; }

        auto& vertical_scrollbar() { return _vert_sbar; }

        void mouse_wheel(const Vector &) override;
        void key_down(const Keycode &) override;

        void render(Canvas_t *, const Point &offset) override;

        // TODO: it may be better to let the pane access the member directly via friend declaration
        auto content_rectangle() const -> const Rectangle & { return _content_rect; } // to be accessed by Scrollable_pane (or derived)

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

        auto pane() { return static_cast<Scrollable_pane_t*>(_content); }

        //Border                  _border    = { 4, {0, 0.2f, 0.6f, 1} };     // encompasses both content area and scrollbar(s)
        Separator               _separator = { 1, {0.2f, 0.2f, 0.2f, 1} };
        Vertical_scrollbar_t    _vert_sbar;
        Rectangle               _content_rect;      // set by layouter
        Position                _vert_sep_pos;      // positions of the separators (vertical = x, horizontal = y)
        Navigation_handler      _on_navigation;
        Scrollable_pane_t      *_content = nullptr;
    };

    // Layouter aspect

    template<class Config, class Pane, class Parent>
    struct Scrollbox__Layouter<Config, true, Pane, Parent>: public Parent
    {
        struct Scrollbox_t: public Scrollbox<Config, true, Pane> { friend struct Scrollbox__Layouter; };
        auto p() { return static_cast<Scrollbox_t*>(static_cast<Scrollbox<Config, true, Pane>*>(this)); }

        auto get_minimal_size() -> Extents override;
        void layout() override;

    protected:
        //auto content_rect() -> Rectangle;
    };

    // Scrollable pane ==============================================

    // Forward declarations

    template<class Config, bool With_layout, class Parent> struct Scrollable_pane__Layouter;

    // Base class

    template<class Config, bool With_layout>
    class Scrollable_pane_base: public Container<Config, With_layout>
    {
    public:
        //using Navigation_handler = Custom_vertical_scrollbar<Config, With_layout>;

        // To implement in CRTP descendant class:

        // TODO: the interface should probably be reduced to the single scroll() method; however initial_pos 
        //      should be handled by the scrollbar
        void scroll(Navigation_unit unit, /* Position initial_pos, */ Fraction<int> delta) { static_assert(false, "Scrollable_pane::navigate()"); }

        /** TODO: inform container scrollbox that the extents of the pane have changed,
            and to update the scrollbar(s) accordingly
         */
        void notify_extents_changed();
    };

    // Main class template

    template<class Config, bool With_layout>
    class Scrollable_pane: 
        public Scrollable_pane__Layouter<Config, With_layout, Scrollable_pane_base<Config, With_layout> >
    {
    protected:
        static constexpr auto element_background_color() { return Color{ 1, 1, 1, 1 }; }
    };

    // Layouter aspect

    template<class Config, class Parent>
    struct Scrollable_pane__Layouter<Config, true, Parent>: public Parent
    {
        struct Scrollable_pane_t: public Scrollable_pane<Config, true> { friend struct Scrollable_pane__Layouter; };
        auto p() { return static_cast<Scrollable_pane_t*>(static_cast<Scrollable_pane<Config, true>*>(this)); }

        /** Because the size of a scrollable pane can by definition exceed that of its container,
            this additional (CRTP) entry point is provided as an occasion for the pane implementation
            to adapt (in whatever way) to the size of the content rectangle of the scrollbox.
            */
        void compute_and_set_extents(const Extents &content_rect) { static_assert(false, "CRPT"); }

        //auto get_minimal_size() -> Extents override;
        //void layout() override;
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_SCROLLBOX(Config, With_layout, PaneType) \
    template cppgui::Scrollbox          <Config, With_layout, PaneType>;
