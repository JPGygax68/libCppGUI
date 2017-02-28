#pragma once

#include <SDL_events.h>

#include "./SDL_exception.hpp"
#include "./SDL_window.hpp"
#include "./SDL_application.hpp"

int SDL_application::run()
{
    SDL_Event ev;
            
    for (;;)
    {
        int err;

        if (SDL_WaitEvent(&ev) != 1) throw SDL_exception("waiting for event");

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
                    SDL_window::dispatch_window_event(ev.window);
                    break;
                case SDL_MOUSEMOTION:
                    SDL_window::dispatch_mousemotion_event(ev.motion);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_window::dispatch_mousebutton_event(ev.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_window::dispatch_mousebutton_event(ev.button);
                    break;
                case SDL_MOUSEWHEEL:
                    SDL_window::dispatch_mousewheel_event(ev.wheel);
                    break;
                case SDL_TEXTINPUT:
                    SDL_window::dispatch_textinput_event(ev.text);
                    break;
                case SDL_KEYDOWN:
                    SDL_window::dispatch_keydown_event(ev.key);
                    break;
                }
            }
        }
        while ((err = SDL_PollEvent(&ev)) == 1);

        SDL_window::for_each_window([](SDL_window *window) { window->end_of_event_burst(); });
    }
}

auto SDL_application::register_custom_event(Custom_event_handler handler) -> uint32_t
{
    auto id = SDL_RegisterEvents(1);

    event_map()[id] = handler;

    return id;
}

auto SDL_application::event_map() -> std::map<uint32_t, Custom_event_handler>&
{
    static std::map<uint32_t, Custom_event_handler> map;

    return map;
}
