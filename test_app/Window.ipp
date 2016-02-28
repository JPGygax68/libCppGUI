#pragma once

#include "./Window.hpp"

#include <stdexcept>
#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_events.h>

#include "./Window.hpp"

static class Static_init {
public:
    Static_init()
    {
        //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
        if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
        atexit(&SDL_VideoQuit);
    }
} _init;

template <class Impl>
auto Window<Impl>::redraw_event_id() -> uint32_t &
{
    static uint32_t id = SDL_RegisterEvents(1);
    return id;
}

template <class Impl>
auto Window<Impl>::window_map() -> std::map<uint32_t, Window*> &
{
    static std::map<uint32_t, Window*> map;

    return map;
}

template <class Impl>
void Window<Impl>::dispatch_event(SDL_WindowEvent &ev)
{
    window_map()[ev.windowID]->handle_event(ev);
}

template <class Impl>
void Window<Impl>::dispatch_redraw(uint32_t win_id)
{
    window_map()[win_id]->request_redraw();
}

template <class Impl>
Window<Impl>::~Window()
{
    window_map().erase(SDL_GetWindowID(_win.get()));
}

template <class Impl>
void Window<Impl>::handle_event(SDL_WindowEvent &ev)
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

template <class Impl>
void Window<Impl>::request_redraw()
{
    std::cout << "Window::request_redraw()" << std::endl;

    SDL_Event ev;
    ev.type = redraw_event_id();
    SDL_UserEvent &ue = ev.user;
    ue.windowID = SDL_GetWindowID(_win.get());
    SDL_PushEvent(&ev);
}

template <class Impl>
Window<Impl>::Window(const std::string &title)
{
    auto win = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

    _win.reset(win);

    window_map()[SDL_GetWindowID(win)] = this;
}

template <class Impl>
void Window<Impl>::Deleter::operator()(SDL_Window * win)
{
    SDL_DestroyWindow(win);
}
