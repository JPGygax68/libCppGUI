#pragma once

#include <locale>
#include <codecvt>

#include "./Window.hpp"

#include <stdexcept>
#include <iostream>
#include <map>

#include <SDL.h>
#include <SDL_events.h>

#include "./error_handling.hpp"
#include "./Application.hpp"
#include "./Window.hpp"


namespace cppgui {

    namespace sdl {

        template <class Impl>
        struct Window<Impl>::Static_init {

            Static_init()
            {
                //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
                if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
                atexit(&SDL_VideoQuit);

                redraw_window = Application<Impl>::register_custom_event([](const SDL_UserEvent &ev) {

                    Window<Impl>::dispatch_redraw(ev.windowID);
                });
            }

            uint32_t redraw_window;
        };
        
        template<class Impl>
        typename Window<Impl>::Static_init Window<Impl>::_initializer;

        template <class Impl>
        Window<Impl>::Window(const std::string &title, int w, int h)
        {
            auto win = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

            _win.reset(win);

            window_map()[id()] = static_cast<Impl*>(this);

            p()->init_graphics();
        }

        template <class Impl>
        Window<Impl>::~Window()
        {
            window_map().erase(id());
        }

        template<class Impl>
        auto Window<Impl>::id() -> uint32_t
        {
            return SDL_GetWindowID(_win.get());
        }

        template<class Impl>
        auto Window<Impl>::create_gl_context() -> SDL_GLContext
        {
            SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
            auto ctx = SDL_GL_CreateContext(_win.get());
            if (!ctx) throw Error("trying to create OpenGL context for a window");
            return ctx;
        }

        template<class Impl>
        void Window<Impl>::delete_gl_context(SDL_GLContext ctx)
        {
            SDL_GL_DeleteContext(ctx);
        }

        template<class Impl>
        void Window<Impl>::make_gl_context_current(SDL_GLContext ctx)
        {
            if (SDL_GL_MakeCurrent(_win.get(), ctx) < 0) throw sdl::Error("trying to make GL context current");
        }

        template<class Impl>
        void Window<Impl>::gl_swap()
        {
            SDL_GL_SwapWindow(_win.get());
        }

        template <class Impl>
        auto Window<Impl>::window_map() -> std::map<uint32_t, Impl*> &
        {
            static std::map<uint32_t, Impl*> map;

            return map;
        }

        template <class Impl>
        void Window<Impl>::dispatch_window_event(SDL_WindowEvent &ev)
        {
            window_map()[ev.windowID]->handle_window_event(ev);
        }

        template<class Impl>
        void Window<Impl>::dispatch_mousemotion_event(SDL_MouseMotionEvent & ev)
        {
            window_map()[ev.windowID]->handle_mousemotion_event(ev);
        }

        template<class Impl>
        void Window<Impl>::dispatch_mousebutton_event(SDL_MouseButtonEvent & ev)
        {
            window_map()[ev.windowID]->handle_mousebutton_event(ev);
        }

        template<class Impl>
        void Window<Impl>::dispatch_mousewheel_event(SDL_MouseWheelEvent & ev)
        {
            window_map()[ev.windowID]->handle_mousewheel_event(ev);
        }

        template<class Impl>
        void Window<Impl>::dispatch_textinput_event(SDL_TextInputEvent & ev)
        {
            window_map()[ev.windowID]->handle_textinput_event(ev);
        }

        template<class Impl>
        void Window<Impl>::dispatch_keydown_event(SDL_KeyboardEvent & ev)
        {
            window_map()[ev.windowID]->handle_keydown_event(ev);
        }

        template <class Impl>
        void Window<Impl>::dispatch_redraw(uint32_t win_id)
        {
            window_map()[win_id]->handle_redraw();
        }

        template <class Impl>
        void Window<Impl>::handle_window_event(SDL_WindowEvent &ev)
        {
            std::cout << "Window::handle_window_event()" << std::endl;

            switch (ev.event)
            {
            case SDL_WINDOWEVENT_RESIZED: 
            case SDL_WINDOWEVENT_SIZE_CHANGED: 
                p()->size_changed(ev.data1, ev.data2);
                invalidate();
                break;
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_EXPOSED:
                invalidate();
                break;
            case SDL_WINDOWEVENT_CLOSE:
                p()->closing();
                break;
            }
        }

        template <class Impl>
        void Window<Impl>::handle_mousemotion_event(SDL_MouseMotionEvent &ev)
        {
            // std::cout << "Window::handle_mousemotion_event()" << std::endl;

            p()->mouse_motion(ev.x, ev.y);
        }

        template<class Impl>
        void Window<Impl>::handle_mousebutton_event(SDL_MouseButtonEvent & ev)
        {
            auto dir = ev.state == SDL_PRESSED ? down : up;
            p()->mouse_button(ev.x, ev.y, ev.button, dir, ev.clicks);
        }

        template<class Impl>
        void Window<Impl>::handle_mousewheel_event(SDL_MouseWheelEvent & ev)
        {
            // TODO: handle ev.direction
            p()->mouse_wheel(ev.x, ev.y);
        }

        template<class Impl>
        void Window<Impl>::handle_textinput_event(SDL_TextInputEvent & ev)
        {
        #if _MSC_VER == 1900
            std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> utf32conv;
            auto utf32 = utf32conv.from_bytes(ev.text);
            p()->text_input(reinterpret_cast<const char32_t *>(utf32.data()), utf32.size());
        #else
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
            auto utf32 = utf32conv.from_bytes(ev.text);
            _text_input_handler(utf32.c_str(), utf32.size());
            p()->text_input(utf32.data(), utf32.size());
        #endif
        }

        template<class Impl>
        void Window<Impl>::handle_keydown_event(SDL_KeyboardEvent &ev)
        {
            p()->key_down(ev.keysym); // TODO: pass "repeat" too ?
        }

        template<class Impl>
        void Window<Impl>::handle_redraw()
        {
            p()->redraw();
        }

        template <class Impl>
        void Window<Impl>::invalidate()
        {
            //std::cout << "Window::request_redraw()" << std::endl;

            SDL_Event ev;
            ev.type = _initializer.redraw_window;
            SDL_UserEvent &ue = ev.user;
            ue.windowID = SDL_GetWindowID(_win.get());
            SDL_PushEvent(&ev);
        }

        template<class Impl>
        auto Window<Impl>::client_extents() const -> Extents
        {
            Extents ext;

            SDL_GetWindowSize(_win.get(), &ext.w, &ext.h);

            return ext;
        }

        template <class Impl>
        void Window<Impl>::Deleter::operator()(SDL_Window * win)
        {
            SDL_DestroyWindow(win);
        }

    } // ns sdl

} // ns cppgui
