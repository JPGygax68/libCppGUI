#pragma once

#include <map>
#include <functional>


struct SDL_UserEvent;

namespace cppgui {
        
    using Custom_event_handler = std::function<void(const SDL_UserEvent &)>;

    extern std::map<uint32_t, Custom_event_handler> custom_events;

    auto register_custom_event(Custom_event_handler handler) -> uint32_t;

} // ns cppgui