#pragma once

#include <SDL_events.h>

#include "./Window.hpp"
#include "./Application.hpp"

#include "./Window.ipp"

namespace cppgui {

    namespace sdl {

        template <class WinT>
        int Application<WinT>::run()
        {
            SDL_Event ev;
            
            for (;;)
            {
                int err;

                if (SDL_WaitEvent(&ev) != 1) throw Error("waiting for event");

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
                            WinT::dispatch_window_event(ev.window);
                            break;
                        case SDL_MOUSEMOTION:
                            WinT::dispatch_mousemotion_event(ev.motion);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            WinT::dispatch_mousebutton_event(ev.button);
                            break;
                        case SDL_MOUSEBUTTONUP:
                            WinT::dispatch_mousebutton_event(ev.button);
                            break;
                        case SDL_MOUSEWHEEL:
                            WinT::dispatch_mousewheel_event(ev.wheel);
                            break;
                        case SDL_TEXTINPUT:
                            WinT::dispatch_textinput_event(ev.text);
                            break;
                        case SDL_KEYDOWN:
                            WinT::dispatch_keydown_event(ev.key);
                            break;
                        }
                    }
                }
                while ((err = SDL_PollEvent(&ev)) == 1);

                WinT::for_each_window([](WinT *window) { window->end_of_event_burst(); });
            }
        }

        template<class WinT>
        auto Application<WinT>::register_custom_event(Custom_event_handler handler) -> uint32_t
        {
            auto id = SDL_RegisterEvents(1);

            event_map()[id] = handler;

            return id;
        }

        template<class WinT>
        auto Application<WinT>::event_map() -> std::map<uint32_t, Custom_event_handler>&
        {
            static std::map<uint32_t, Custom_event_handler> map;

            return map;
        }

    } // ns sdl

} // ns cppgui
