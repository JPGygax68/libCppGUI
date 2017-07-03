#pragma once

#include <cppgui_config.hpp>
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Rectangle.hpp"


namespace cppgui
{
    class Canvas;


    class ISurface
    {
    public:

        virtual ~ISurface() {}

        virtual void invalidate() = 0;

        virtual void init(void *context) = 0;
        virtual void cleanup(void *context) = 0;

        virtual void canvas_created(Canvas *) = 0;
            // Must be called by implementor when canvas was created
        virtual void before_destroy_canvas(Canvas *) = 0;
            // Mus be called by implementor before destroying canvas

        virtual void render_ui(Canvas *) = 0;

        virtual void show() = 0;
            // May be unnecessary, but must be called to ensure surface will be shown.

        virtual auto rectangle() -> Rectangle = 0;

        virtual void add_popup(ISurface *) = 0;
        virtual void remove_popup(ISurface *) = 0;

        virtual void size_changed(const Extents &) = 0;
        virtual void text_input(const char32_t *text, size_t cp_count) = 0;
        virtual void key_down(Keycode key) = 0;
        virtual void mouse_motion(const Point &) = 0;
        virtual void mouse_button(const Point &p, uint btn, Key_state, uint clicks) = 0;
        virtual void mouse_wheel(const Point &) = 0;
    };  
    
} // ns cppui

