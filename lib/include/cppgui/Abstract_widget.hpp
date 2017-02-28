#pragma once

// OBSOLETE - DO NOT USE

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

#include <functional>

#include <cppgui_config.hpp>

#include "./basic_types.hpp"
#include "./Canvas.hpp"
#include "./Resource.hpp"

#include CPPGUI_RENDERER_HEADER
#include CPPGUI_INPUT_ADAPTER_HEADER


namespace cppgui {

    // Forward declarations 

    class Root_widget_base;
    class Abstract_container;

    /*template <class Config, bool With_layout>*/ class Drag_controller;

    /** 
     *  Abstract_widget: functionality common to both Root_widget and Widget, i.e. not including the ability
     *  to function as an element in a container.
     */
    class Abstract_widget
    {
    public:
        virtual ~Abstract_widget() {}

        using Keycode           = Keyboard_adapter::Keycode;
        using Native_color      = Renderer::Native_color;

        #ifdef OBSOLETE
        using Abstract_widget_t = Abstract_widget;
        using Root_widget_t     = Root_widget_base;
        using Font_handle       = typename Canvas::Font_handle;
        using Keyboard          = typename Config::Keyboard;
        // TODO: move the following resource type definitions into a special struct and inherit from that ?
        using Color_resource    = Resource<const RGBA &, Native_color, Canvas, true>;
        using Font_resource     = Resource<const Rasterized_font *, Font_handle, Canvas, false>;
        #endif

        using Click_handler     = std::function<void(const Point &, int button, Count clicks)>; // TODO: support return value ?
        using Pushed_handler    = std::function<void()>; // for buttons TODO: renamed event to "Push" (as in "a push happened") ?

        void set_id(const char *id)
        {
            #ifdef _DEBUG
            _id = id;
            #endif
        }

        auto& rectangle() { return _rect; }
        auto& rectangle() const { return _rect; }
        auto& position() { return _rect.pos; }
        auto& position() const { return _rect.pos; }
        auto& extents() { return _rect.ext; }
        auto& extents() const { return _rect.ext; }
        void set_position(const Point &);
        void set_extents(const Extents &);

        /** The init() entry point is where a widget "connects" to its backends (the most important of
        which being the canvas).
        */
        virtual void init() {}

        /** The compute_view_from_data() entry point must be called after init(), and also after 
            layout() if run-time layouting is enabled.
        */
        virtual void compute_view_from_data() {}

        // Input event injection

        virtual void mouse_enter() {}       // TODO: provide "entry point" parameter ?
        virtual void mouse_exit() {}        // TODO: provide "exit point" parameter ?

        bool disabled() const { return false; } // TODO!!!

        /** Convention: the provided position is an offset to be added to the widget's
            own coordinates.
        */
        virtual void render(Canvas *, const Point &offset) = 0;

        virtual bool handle_key_down(const Keycode &) { return false; }

    protected:

        // Rendering conveniences

        // auto rgba_to_native(Canvas *, const RGBA &) -> Native_color;
        auto rgba_to_native(const RGBA &) -> Native_color;
        void fill_rect(Canvas *, const Rectangle &rect, const Native_color &);
        void fill_rect(Canvas *, const Rectangle &rect, const Point &offs, const Native_color &);
        void fill_rect(Canvas *, const Point &pos, const Extents &ext, const Native_color &);
        void fill(Canvas *, const Point &offs, const Native_color &);
        auto convert_position_to_inner(const Point &) -> Point;
        auto advance_to_glyph_at(const Rasterized_font *, const std::u32string &text, size_t from, size_t to, Point &pos) 
            -> const Glyph_control_box *;
        void draw_borders(Canvas *, const Point & offs, Width width, const RGBA &color);
        void draw_borders(Canvas *, const Rectangle &rect, const Point &offs, Width width, const RGBA &color);
        void draw_borders(Canvas *, const Rectangle &rect, const Point &offs, 
            Width width, const RGBA & top, const RGBA & right, const RGBA & bottom, const RGBA & left);
        // PROVISIONAL
        //void draw_stippled_inner_rect(Canvas *, const Rectangle &, const Point &offs);

        // Experimental & temporary: implement more sophisticated (and flexible!) styling
        // - May not / should not stay static; make const if possible

        static auto stroke_width() -> int { return 1; }
        static auto stroke_color() -> RGBA { return { 0, 0, 0, 1 }; }
        //static auto padding() -> int { return 5; }
        static auto paper_color() -> RGBA { return {1, 1, 1, 1}; }

    private:

        #ifdef _DEBUG
        const char  *_id;
        #endif

        Rectangle   _rect = {};
    };

} // ns cppgui
