#pragma once

#include <SDL_events.h>

#include "./Window.hpp"
#include "./Application.hpp"

namespace cppgui {

    namespace sdl {

        template <class WinT>
        int Application<WinT>::run()
        {
            SDL_Event ev;

            while (SDL_WaitEvent(&ev))
            {
                // Custom event ?
                auto it = _custom_events_map.find(ev.user.type);
                if (it != _custom_events_map.end())
                {
                    it->second(ev.user);
                    return;
                }

                switch (ev.type)
                {
                    case SDL_QUIT: 
                        return;
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
                    /*
                    case WinT::redraw_event_id():
                        WinT::dispatch_redraw(ev.user.windowID);
                        break;
                    */
                    default:
                        assert(false && "unknown SDL event type");
                }
            }

            return -1;
        }

        template<class WinT>
        auto cppgui::sdl::Application<WinT>::register_custom_event(Custom_event_handler handler) -> uint32_t
        {
            auto id = SDL_RegisterEvents(count);

            _custom_events_map[id] = handler;
        }

    } // ns sdl

} // ns cppgui
