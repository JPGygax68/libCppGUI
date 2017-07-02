#pragma once

#include <cppgui/Rectangle.hpp>


namespace cppgui
{
    class Canvas;


    class ISurface
    {
    public:

        virtual ~ISurface() {}

        virtual auto rectangle() -> Rectangle = 0;

        virtual void invalidate() = 0;

        virtual void add_popup(ISurface *) = 0;
        virtual void remove_popup(ISurface *) = 0;

        virtual void render(Canvas *) = 0;
    };  
    
} // ns cppui

