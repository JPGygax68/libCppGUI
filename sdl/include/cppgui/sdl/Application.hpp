#pragma once

#include <cstdint>
#include <functional>
#include <map>

struct SDL_UserEvent;

namespace cppgui {

    namespace sdl {
    
        template <class WinT>
        class Application {
        public:
            using Custom_event_handler = std::function<void(const SDL_UserEvent &)>;

            int run();

            static auto register_custom_event(Custom_event_handler) -> uint32_t;

            // void post_custom_event(const SDL_UserEvent &);

        private:
            void dispatch_redraw(SDL_UserEvent &);

            static auto event_map() -> std::map<uint32_t, Custom_event_handler> &;
        };

    } // ns sdl

} // ns cppgui