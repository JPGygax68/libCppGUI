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

#include <cppgui_config.hpp>
#include CPPGUI_RENDERER_HEADER
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "./Positioned_bbox.hpp"
#include "./Canvas.hpp"


namespace cppgui {
 
    class Root_widget;


    class UI_element
    {
    public:

        virtual ~UI_element() {}

        // TODO: is it a good idea to give direct access to bounds() (+ origin(), bbox()), or should this be encapsulated ? or friend-accessible only ?
        auto rectangle() const -> Rectangle { return Rectangle{_bounds.bbox} + _bounds.orig; }; 
        auto& origin() { return _bounds.orig; }
        auto& origin() const { return _bounds.orig; }
        auto& bbox() { return _bounds.bbox; }
        auto& bbox() const { return _bounds.bbox; }
        auto bounds() const { return _bounds; }
        auto& bounds() { return _bounds; }
        auto width () const { return rectangle().width (); }
        auto height() const { return rectangle().height(); }

        void move_by(const Point &);
        void shift_by(const Point &);

        /** The init() entry point is where a UI element "connects" to its backends (the most important of
            which being the canvas).
        */
        virtual void init(Canvas *) {}
        
        // Run-time states

        void set_visible(bool visible = true);
        bool visible() const { return _visible; }

        // Queries

        bool hovered() const { return _hovered; }
        virtual auto absolute_position() -> Point = 0;

        // Input event injection

        /*
            By convention, mouse positions are passed to UI elements as relative to
            their own origin (meaning that it falls to the caller, i.e. usually
            the container, to subtract the child widget's position() from the
            coordinates it gets from yet higher up).

            Event handling methods must report whether or not the event was "consumed"
            via the boolean return value.
         */
        virtual bool mouse_motion(const Point &) { return false; }
        virtual bool mouse_button(const Point &, int /*button*/, Key_state, Count clicks) { return false; }
        virtual bool mouse_click(const Point &, int button, Count count) { return false; }
        virtual bool mouse_wheel(const Vector &) { return false; }
        virtual bool text_input(const char32_t *, size_t) { return false; }
        virtual bool key_down(const Keycode &) { return false; }
        //virtual bool key_up(const Keycode &) { return false; }

        // Derived ("secondary") events

        virtual void mouse_enter();
        virtual void mouse_exit();

        // Rendering

        virtual void render(Canvas *, const Point &offset) = 0;

    protected:

        virtual void invalidate() = 0;
            // Invalidation depends on the type of UI element, so it is only a contract here.
            // NOTE: (future) may be made optional via preprocessor configuration

        // Graphics system integration

        void shift_horizontally(Position_delta);
        void shift_vertically(Position_delta);
        void shift_up  (Length);
        void shift_down(Length);

    private:

        //Point                   _origin = {};
        //Bbox                    _bbox = {};
        Positioned_bbox         _bounds = {};
        bool                    _visible = true;
        bool                    _hovered = false;
    };

} // ns cppgui