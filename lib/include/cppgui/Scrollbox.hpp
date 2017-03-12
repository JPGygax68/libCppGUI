#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016, 2017 Hans-Peter Gygax

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
#include "./Vertical_scrollbar.hpp"


namespace cppgui {

    // Forward declarations
    class Scrollable_pane;


    // Class definition

    // TODO: rename to Scrollbox_base ?

    // TODO: this is very much incomplete!

    // TODO: box
    class Scrollbox: public Container
    {
    public:

        using Navigation_handler = std::function<void(Navigation_unit)>;

        Scrollbox();

        void compute_view_from_data() override;

        void on_navigation(Navigation_handler);

        void set_content_pane(Scrollable_pane *);
        auto content_pane() { return _content; }

        auto& vertical_scrollbar() { return _vert_sbar; }

        void mouse_wheel(const Vector &) override;
        void key_down(const Keycode &) override;

        void render(Canvas *, const Point &offset) override;

        // TODO: it may be better to let the pane access the member directly via friend declaration
        auto content_rectangle() const -> const Rectangle & { return _content_rect; } // to be accessed by Scrollable_pane (or derived)

    protected:
        auto pane() { return static_cast<Scrollable_pane*>(_content); }

        //Border                  _border    = { 4, {0, 0.2f, 0.6f, 1} };     // encompasses both content area and scrollbar(s)
        Separator               _separator = { 1, {0.2f, 0.2f, 0.2f, 1} };
        Vertical_scrollbar      _vert_sbar;
        Rectangle               _content_rect;      // set by layouter
        Position                _vert_sep_pos;      // positions of the separators (vertical = x, horizontal = y)
        Navigation_handler      _on_navigation;
        Scrollable_pane         *_content = nullptr;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        auto get_minimal_bounds() -> Bounding_box override;
        void set_bounds(const Point &, const Bounding_box &) override;

        protected:
            //auto content_rect() -> Rectangle;

    #endif // CPPGUI_EXCLUDE_LAYOUTING

    }; // class Scrollbox

    // Scrollable pane ==============================================

    // Base class

    class Scrollable_pane_base: public Container
    {
    public:
        //using Navigation_handler = Custom_vertical_scrollbar<Config, With_layout>;

        // To implement in CRTP descendant class:

        // TODO: the interface should probably be reduced to the single scroll() method; however initial_pos 
        //      should be handled by the scrollbar
        virtual void scroll(Navigation_unit, /* Position initial_pos, */ Fraction<int> delta) = 0;

        /** TODO: inform container scrollbox that the extents of the pane have changed,
            and to update the scrollbar(s) accordingly
         */
        void notify_extents_changed();
    };

    // Main class template

    class Scrollable_pane: public Scrollable_pane_base
    {
    protected:
        static constexpr auto element_background_color() { return RGBA{ 1, 1, 1, 1 }; }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

        /** Because the size of a scrollable pane can by definition exceed that of its container,
            this additional entry point is provided as an occasion for the pane implementation
            to adapt (in whatever way) to the size of the content rectangle of the scrollbox.
            */
        virtual void compute_and_set_extents(const Extents &content_rect) = 0;

        //auto get_minimal_size() -> Extents override;
        //void layout() override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui

