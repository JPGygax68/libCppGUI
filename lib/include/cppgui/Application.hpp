#pragma once

#include <cppgui_config.hpp>


namespace cppgui {
    
    class Application
    {
    public:

        void run();

        void run_unconstrained();

    protected:

        virtual void idle() {}
    };
}
