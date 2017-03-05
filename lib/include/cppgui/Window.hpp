#pragma once

#include "cppgui_config.hpp"

#include CPPGUI_PLATFORM_ADAPTER_HEADER


namespace cppgui {
    
    class Window: public CPPGUI_WINDOW_BASE_CLASS
    {
    protected:
        
        void init_window() override;
        void cleanup_window() override;

        void mouse_motion(int x, int y) override;
    };

} // ns cppgui