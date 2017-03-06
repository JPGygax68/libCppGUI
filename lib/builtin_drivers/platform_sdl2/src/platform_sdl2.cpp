#pragma once

#include <SDL_events.h>
#include "./SDL2_exception.hpp"
#include "./SDL2_window.hpp"
#include "./custom_events.hpp"


namespace cppgui
{
    /*
    * Return true if application is requested to stop.
    */
    static bool handle_event(SDL_Event &ev)
    {
        // Custom event ?
        auto it = custom_events.find(ev.type);
        if (it != custom_events.end())
        {
            it->second(ev.user);
        }
        else 
        {
            switch (ev.type)
            {
            case SDL_QUIT: 
                return true; // TODO: is this the right return value ?
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
            default:
                // TODO: log?
                break;
            }
        }

        return false;
    }

    // PUBLIC API -------------------------------------------------------------

    bool perform_pending_main_thread_tasks(bool wait)
    {
        SDL_Event ev;

        // Either wait for event or poll
        if (wait)
        {
            if (SDL_WaitEvent(&ev) != 0) throw SDL2_exception("waiting for event");
        }
        else
        {
            if (SDL_PollEvent(&ev) == 0) return false;
        }

        // Handle all available events in one burst
        do
        {
            if (handle_event(ev) == true) return true;
        }
        while (SDL_PollEvent(&ev) == 1);

        // Signal end of even burst (good time for a redraw)
        SDL2_window::for_each_window([](SDL2_window *window) { window->end_of_event_burst(); });

        return false;
    }

} // ns cppgui
