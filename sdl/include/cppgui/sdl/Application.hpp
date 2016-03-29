#pragma once

#include <cstdint>
#include <functional>
#include <map>

class SDL_UserEvent;

namespace cppgui {

    namespace sdl {
    
        template <class WinT>
        class Application {
        public:
            using Custom_event_handler = std::function<void(SDL_UserEvent &)>;

            int run();

            auto register_custom_event(Custom_event_handler) -> uint32_t;

            void post_custom_event(const SDL_UserEvent &);

        private:
            std::map<uint32_t, Custom_event_handler>   _custom_events_map;
        };

    } // ns sdl

} // ns cppgui