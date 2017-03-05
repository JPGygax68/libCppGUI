#pragma once

#include <SDL_events.h>

#include "./SDL2_exception.hpp"
#include "./SDL2_window.hpp"
#include "./SDL2_application.hpp"


namespace cppgui
{
    namespace sdl2
    {
        int SDL2_application::run()
        {
            SDL_Event ev;

            for (;;)
            {
                int err;

                if (SDL_WaitEvent(&ev) != 1) throw SDL2_exception("waiting for event");

                do 
                {
                    // Custom event ?
                    auto it = event_map().find(ev.type);
                    if (it != event_map().end())
                    {
                        it->second(ev.user);
                    }
                    else 
                    {
                        switch (ev.type)
                        {
                        case SDL_QUIT: 
                            return 1; // TODO: is this the right return value ?
                        case SDL_WINDOWEVENT:
                            SDL2_window::dispatch_window_event(ev.window);
                            break;
                        case SDL_MOUSEMOTION:
                            SDL2_window::dispatch_mousemotion_event(ev.motion);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            SDL2_window::dispatch_mousebutton_event(ev.button);
                            break;
                        case SDL_MOUSEBUTTONUP:
                            SDL2_window::dispatch_mousebutton_event(ev.button);
                            break;
                        case SDL_MOUSEWHEEL:
                            SDL2_window::dispatch_mousewheel_event(ev.wheel);
                            break;
                        case SDL_TEXTINPUT:
                            SDL2_window::dispatch_textinput_event(ev.text);
                            break;
                        case SDL_KEYDOWN:
                            SDL2_window::dispatch_keydown_event(ev.key);
                            break;
                        }
                    }
                }
                while ((err = SDL_PollEvent(&ev)) == 1);

                SDL2_window::for_each_window([](SDL2_window *window) { window->end_of_event_burst(); });
            }
        }

        auto SDL2_application::register_custom_event(Custom_event_handler handler) -> uint32_t
        {
            auto id = SDL_RegisterEvents(1);

            event_map()[id] = handler;

            return id;
        }

        auto SDL2_application::event_map() -> std::map<uint32_t, Custom_event_handler>&
        {
            static std::map<uint32_t, Custom_event_handler> map;

            return map;
        }
    }
}
