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
#include "./Box.hpp"


namespace cppgui {

    /** Container functionality (ability to contain Widgets).
     */
    class Container_base: public Widget
    {
    public:

        void set_initial_focus(Widget *);

        auto& children() { return _children; }

        void add_child(Widget *);
        // TODO: should removal methods be moved to optional aspect ?
        void remove_child(Widget *);
        void remove_all_children();

        void init(Canvas *) override;

        void render(Canvas *, const Point &offs) override;

        void mouse_motion(const Point &) override;
        void mouse_button(const Point &, int /*button*/, Key_state, Count clicks) override;
        // TODO: other events ?
        void mouse_exit() override;

        /** Called when a key_down event could not be handled by the child it was sent to
            (from container_key_down()) and needs to "bubble" back up.
         */
        virtual void child_key_down(const Keycode &);

        virtual bool container_has_focus() { return has_focus(); }
        virtual auto container_absolute_position() -> Point ;
        virtual void switch_focused_child(Widget *);
        auto focused_child() const -> Widget * { return _focused_child; }

        auto first_child() -> Widget * { assert(!_children.empty()); return _children.front(); }
        auto last_child () -> Widget * { assert(!_children.empty()); return _children.back (); }

        auto child_index(Widget *child) -> Index;

        template<class Pred> auto scan_children_forward (Index from, Pred) -> Index;
        template<class Pred> auto scan_children_backward(Index from, Pred) -> Index;

    protected:

        bool contains_widget(Widget *);
        auto child_at(const Point &) -> Widget *;

        void init_child_resources(Canvas *);
        void compute_child_views();

        void render_children(Canvas *, const Point &offs);

        /** The container_xxxx() methods are intended as "delegate" event handlers, to be 
            called from "real" containers (i.e. descendants of Container<>).            

            TODO: those methods where defined in Abstract_container, which no longer
                exists. Unless such a parent class is reintroduced, the container_xxx()
                methods can be merged back into the corresponding event handlers defined
                in the Widget class.
        */
        void container_mouse_motion(const Point &);
        void container_mouse_button(const Point &, int button, Key_state, Count clicks);
        //void container_mouse_click(const Point &, int button, int count);
        void container_mouse_wheel(const Vector & dist);
        void container_mouse_exit();
        void container_text_input(const char32_t *, size_t);
        bool container_key_down(const Keycode &);

        std::vector<Widget*> _children;
        Widget *_hovered_child = nullptr;
        Widget *_focused_child = nullptr;

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
