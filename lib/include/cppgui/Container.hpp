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

#include "./Widget.hpp"


namespace cppgui {

    /** Container functionality (ability to contain Widgets).
     */
    class Container_base: public Widget
    {
    public:

        void set_initial_focus(Widget *);

        auto& children() { return _children; }
        auto child(Index i) { return _children[i]; }
        auto child_count() const -> Index { return static_cast<Index>(_children.size()); }

        void add_child(Widget *);
        // TODO: should removal methods be moved to optional aspect ?
        void remove_child(Widget *);
        void remove_all_children();

        void compute_view_from_data() override;

        void init(Canvas *) override;

        void render(Canvas *, const Point &offs) override;

        // Event handling

        bool mouse_motion(const Point &) override;
        bool mouse_button(const Point &, int /*button*/, Key_state, Count clicks) override;
        // TODO: other events ?
        bool mouse_wheel(const Vector &v) override;
        bool text_input(const char32_t *, size_t) override;
        bool key_down(const Keycode &) override;

        void mouse_exit() override;

        // Actions

        virtual bool cycle_focus(int steps);

        void set_focus_on_child(Widget *);

        // Queries

        virtual auto container_absolute_position() -> Point ;
        auto focused_child() const -> Widget * { return _focused_child; }

        auto first_child() -> Widget * { assert(!_children.empty()); return _children.front(); }
        auto last_child () -> Widget * { assert(!_children.empty()); return _children.back (); }

        auto child_index(Widget *child) -> Index;

        template<class Pred> auto scan_children_forward (Index from, Pred) -> Index;
        template<class Pred> auto scan_children_backward(Index from, Pred) -> Index;

    protected:
        using Super = Widget;
        using Child_list = std::vector<Widget*>;

        bool contains_widget(Widget *);
        auto child_at(const Point &) -> Widget *;

        void init_child_resources(Canvas *);
        void compute_child_views();

        void render_children(Canvas *, const Point &offs);

        bool cycle_focus_forward ();
        bool cycle_focus_backward();

        Child_list              _children;
        Widget                 *_hovered_child = nullptr;
        Widget                 *_focused_child = nullptr;

    //-----------------------------------------------------
    // Container Updater "aspect"

    public:

        virtual void child_invalidated(Widget *);
        virtual auto container_root_widget() -> Root_widget * { return root_widget(); }

    // END of Container Updater aspect
    //-----------------------------------------------------

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;

    protected:
        void init_children_layout();
        //auto get_minimal_bounds() -> Bounding_box override;

    #endif // CPPGUI_EXCLUDE_LAYOUTING

    };


#ifdef NOT_DEFINED

    class Container_box_styles
    {
    protected:
        // TODO: make configurable:
        static auto border_width    (Widget_states) -> Width { return 1; }
        static auto padding_width   (Widget_states) -> Width { return 4; }
        static auto border_color    (Widget_states) -> RGBA { return { 0, 0, 0, 1 }; }
        static auto background_color(Widget_states) -> RGBA { return { 0.7f, 0.7f, 0.7f, 1}; }
    };

#endif // NOT_DEFINED

} // ns cppgui
