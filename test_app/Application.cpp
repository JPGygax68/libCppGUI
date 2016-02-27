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
    }

    return -1;
}
