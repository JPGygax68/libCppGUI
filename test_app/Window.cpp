#include <stdexcept>
#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_events.h>

#include "./Window.hpp"

static std::map<Uint32, Window_base*> windows;

Uint32 Window_base::redraw_event_id;

static class Window_base::Static_init {
public:
    Static_init()
    {
        //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
        if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
        atexit(&SDL_VideoQuit);
        redraw_event_id = SDL_RegisterEvents(1);
    }
} _init;

template <class Impl>
void Window::dispatch_event(SDL_WindowEvent &ev)
{
    windows[ev.windowID]->handle_event(ev);
}

void Window::dispatch_redraw(uint32_t win_id)
{
    windows[win_id]->request_redraw();
}

Window::~Window()
{
    windows.erase(SDL_GetWindowID(_win.get()));
}

void Window::handle_event(SDL_WindowEvent &ev)
{
    std::cout << "Window::handle_event()" << std::endl;

    switch (ev.event)
    {
    case SDL_WINDOWEVENT_SHOWN: 
    case SDL_WINDOWEVENT_MAXIMIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        request_redraw();
        break;
    }
}

void Window::request_redraw()
{
    std::cout << "Window::request_redraw()" << std::endl;

    SDL_Event ev;
    ev.type = redraw_event_id;
    SDL_UserEvent &ue = ev.user;
    ue.windowID = SDL_GetWindowID(_win.get());
    SDL_PushEvent(&ev);
}

Window::Window(const std::string &title)
{
    auto win = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

    _win.reset(win);

    windows[SDL_GetWindowID(win)] = this;
}

void Window::Deleter::operator()(SDL_Window * win)
{
    SDL_DestroyWindow(win);
}
