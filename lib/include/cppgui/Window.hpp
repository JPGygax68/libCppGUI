#pragma once

#include <memory>

#include "cppgui_config.hpp"

#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"


namespace cppgui {
    
    class Window: public CPPGUI_WINDOW_BASE_CLASS
    {
    protected:
        
        void init_graphics(void *context) override;
        void cleanup_graphics(void *context) override;
        void redraw(void *context) override;

        void mouse_motion(int x, int y) override;

    private:
        std::unique_ptr<Canvas>     _canvas;
    };

} // ns cppgui