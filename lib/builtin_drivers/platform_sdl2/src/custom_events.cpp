#include <SDL2/SDL.h>
#include <sdl2_custom_events.hpp>


namespace cppgui {

    namespace sdl {

        auto custom_events() -> std::map<uint32_t, sdl::Custom_event_handler> & 
        {
            static std::map<uint32_t, sdl::Custom_event_handler> events;
            return events;
        }
    
        auto register_custom_event(sdl::Custom_event_handler handler) -> uint32_t
        {
            auto id = SDL_RegisterEvents(1);
            custom_events()[id] = handler;
            return id;
        }

    } // ns sdl

} // ns cppgui
