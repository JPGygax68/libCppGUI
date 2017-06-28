#pragma once

namespace cppgui
{

    class ISurface
    {
    public:

        virtual void invalidate() = 0;

    protected:

        virtual void redraw(void *context) = 0;
    };  
    
} // ns cppui

