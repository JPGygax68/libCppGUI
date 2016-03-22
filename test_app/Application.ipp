#pragma once

#include <SDL_events.h>

#include "./Window.hpp"
#include "./Application.hpp"

template <class WinT>
int Application<WinT>::run()
{
    SDL_Event ev;

    while (SDL_WaitEvent(&ev))
    {
        if (ev.type == SDL_QUIT)
        {
            break;
        }
        else if (ev.type == SDL_WINDOWEVENT)
        {
            WinT::dispatch_window_event(ev.window);
        }
        else if (ev.type == SDL_MOUSEMOTION)
        {
            WinT::dispatch_mousemotion_event(ev.motion);
        }
        else if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            WinT::dispatch_mousebutton_event(ev.button);
        }
        else if (ev.type == SDL_MOUSEBUTTONUP)
        {
            WinT::dispatch_mousebutton_event(ev.button);
        }
        else if (ev.type == SDL_TEXTINPUT)
        {
            WinT::dispatch_textinput_event(ev.text);
        }
        else if (ev.type == SDL_KEYDOWN)
        {
            WinT::dispatch_keydown_event(ev.key);
        }
        else if (ev.type == WinT::redraw_event_id())
        {
            WinT::dispatch_redraw(ev.user.windowID);
        }
    }

    return -1;
}
