#pragma once

#include <map>
#include <functional>


struct SDL_UserEvent;

namespace cppgui {
        
    namespace sdl
    {
        
        using Custom_event_handler = std::function<void(const SDL_UserEvent &)>;

        auto custom_events() -> std::map<uint32_t, Custom_event_handler> &;

        auto register_custom_event(Custom_event_handler handler) -> uint32_t;

    } // ns sdl

} // ns cppgui