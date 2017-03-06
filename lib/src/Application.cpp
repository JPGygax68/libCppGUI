#include "Application.hpp"
#include CPPGUI_PLATFORM_ADAPTER_HEADER


namespace cppgui
{

    void Application::run()
    {
        // TODO: whether or not to wait for events should be determined by configuration and/or user choice
        while (perform_pending_main_thread_tasks(true))
        {
            ;
        }
    }

} // ns cppgui