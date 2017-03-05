#include "cppgui_config.hpp"
#include CPPGUI_RENDERER_HEADER

#include "Window.hpp"


namespace cppgui {
    
    void Window::init_window()
    {
        // Renderer adapter
        CPPGUI_RENDERER_CLASS::init();
        // TODO: other subsystem adapters
    }

    void Window::cleanup_window()
    {
        CPPGUI_RENDERER_CLASS::cleanup();
        // TODO: other subsystem adapters
    }

} // ns cppgui