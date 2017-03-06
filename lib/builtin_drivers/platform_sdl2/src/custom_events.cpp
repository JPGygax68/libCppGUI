#include <SDL2/SDL.h>
#include "./custom_events.hpp"


namespace cppgui {

    auto custom_events() -> std::map<uint32_t, Custom_event_handler> & 
    {
        static std::map<uint32_t, Custom_event_handler> events;
        return events;
    }
    
    auto register_custom_event(Custom_event_handler handler) -> uint32_t
    {
        auto id = SDL_RegisterEvents(1);
        custom_events()[id] = handler;
        return id;
    }

} // ns cppgui
