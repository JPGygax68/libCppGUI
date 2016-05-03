#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <set>

union SDL_Event;
struct SDL_UserEvent;

namespace cppgui {

    namespace sdl {
    
        template <class WinT>
        class Application {
        public:
            using Custom_event_handler = std::function<void(const SDL_UserEvent &)>;

            // Concept implementation

            int run();

            // Additional methods

            // TODO: overload that changes the handler but keeps the event id
            static auto register_custom_event(Custom_event_handler) -> uint32_t;
            //static auto register_custom_window_event(Custom_window_event_handler) -> uint32_t;

            // void post_custom_event(const SDL_UserEvent &);

        private:

            static auto event_map() -> std::map<uint32_t, Custom_event_handler> &;

            //static auto instance_pointer() -> Application * &;

            std::set<WinT*>     windows;
        };

    } // ns sdl

} // ns cppgui