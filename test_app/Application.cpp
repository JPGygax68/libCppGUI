#include <SDL_events.h>

#include "./Window.hpp"
#include "./Application.hpp"

int Application::run()
{
    auto win = Window::create();

    SDL_Event ev;

    while (SDL_WaitEvent(&ev))
    {
        if (ev.type == SDL_QUIT)
        {
            break;
        }
        else if (ev.type == SDL_WINDOWEVENT)
        {
            Window::dispatch_event(ev.window);
        }
        else if (ev.type == Window::redraw_event_id)
        {
            Window::dispatch_redraw(ev.user.windowID);
        }
    }

    return -1;
}
