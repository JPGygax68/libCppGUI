#include <iostream>
#include <locale>
#include <codecvt>

#include <SDL.h>
#include <SDL_events.h>
#include "platform_sdl2.hpp"
#include "SDL2_window.hpp"
#include "SDL2_exception.hpp"
#include "custom_events.hpp"


namespace cppgui {
    
    struct Static_init {

        Static_init()
        {
            //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
            if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
            atexit(&SDL_VideoQuit);

            redraw_window = register_custom_event([](const SDL_UserEvent &ev) {

                SDL2_window::dispatch_redraw(ev.windowID);
            });
        }

        uint32_t redraw_window;
    };

    Static_init _initializer;

    SDL2_window::SDL2_window(const std::string &title, int w, int h)
    {
        // TODO: make OpenGL optional (according to configuration)
        auto win = SDL_CreateWindow(title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
            /* SDL_WINDOW_FULLSCREEN_DESKTOP | */ SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

        _win.reset(win);

        #ifdef CPPGUI_USING_OPENGL
        _gr_ctx = SDL_GL_CreateContext(win);
        #endif

        window_map()[id()] = this; // static_cast<SDL2_window*>(this);
    }

    SDL2_window::~SDL2_window()
    {
        window_map().erase(id());
    }

    void SDL2_window::init()
    {
        // Graphics
        #ifdef CPPGUI_USING_OPENGL
        SDL_GL_MakeCurrent(_win.get(), _gr_ctx);
        #else
        #error No or unsupported graphics library defined
        #endif
        init_graphics(_gr_ctx);

        // TODO: other subsystems
    }

    void SDL2_window::cleanup()
    {
        #ifdef CPPGUI_USING_OPENGL
        SDL_GL_MakeCurrent(_win.get(), _gr_ctx);
        #else
        #error No or unsupported graphics library defined
        #endif
    }

    auto SDL2_window::id() -> uint32_t
    {
        return SDL_GetWindowID(_win.get());
    }

    void SDL2_window::begin_rendering()
    {
        SDL_GL_MakeCurrent(_win.get(), _gr_ctx);
    }

    void SDL2_window::done_rendering()
    {
        SDL_GL_SwapWindow(_win.get());
    }

    auto SDL2_window::get_current_gl_context() -> SDL_GLContext
    {
        return SDL_GL_GetCurrentContext();
    }

    auto SDL2_window::create_gl_context() -> SDL_GLContext
    {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        auto ctx = SDL_GL_CreateContext(_win.get());
        if (!ctx) throw SDL2_exception("trying to create OpenGL context for a window");
        return ctx;
    }

    void SDL2_window::delete_gl_context(SDL_GLContext ctx)
    {
        SDL_GL_DeleteContext(ctx);
    }

    void SDL2_window::make_gl_context_current(SDL_GLContext ctx)
    {
        //if (SDL_GL_MakeCurrent(_win.get(), ctx) < 0) throw sdl::Error("trying to make GL context current");
        int err;
        int count;
        for (count = 0; ;)
        {
            count ++;
            err = SDL_GL_MakeCurrent(_win.get(), ctx);
            if (err == 0) break;
            if (count >= 10) throw SDL2_exception("trying to make GL context current (repeated attempts to call SDL_GL_MakeCurrent())");
            SDL_Delay(1);
        }
        if (count > 1) std::cerr << "SDL2_window::make_gl_context_current() has required " << count << " attempts" << std::endl;
    }

    void SDL2_window::gl_swap()
    {
        SDL_GL_SwapWindow(_win.get());
    }

    auto SDL2_window::window_map() -> std::map<uint32_t, SDL2_window*> &
    {
        static std::map<uint32_t, SDL2_window*> map;

        return map;
    }

    // TODO: protect all dispatchers again ev.windowID == 0 

    void SDL2_window::dispatch_window_event(SDL_WindowEvent &ev)
    {
        window_map()[ev.windowID]->handle_window_event(ev);
    }

    void SDL2_window::dispatch_mousemotion_event(SDL_MouseMotionEvent & ev)
    {
        window_map()[ev.windowID]->handle_mousemotion_event(ev);
    }

    void SDL2_window::dispatch_mousebutton_event(SDL_MouseButtonEvent & ev)
    {
        window_map()[ev.windowID]->handle_mousebutton_event(ev);
    }

    void SDL2_window::dispatch_mousewheel_event(SDL_MouseWheelEvent & ev)
    {
        window_map()[ev.windowID]->handle_mousewheel_event(ev);
    }

    void SDL2_window::dispatch_textinput_event(SDL_TextInputEvent & ev)
    {
        window_map()[ev.windowID]->handle_textinput_event(ev);
    }

    void SDL2_window::dispatch_keydown_event(SDL_KeyboardEvent & ev)
    {
        window_map()[ev.windowID]->handle_keydown_event(ev);
    }

    void SDL2_window::dispatch_redraw(uint32_t win_id)
    {
        window_map()[win_id]->handle_redraw();
    }

    void SDL2_window::dispatch_custom_event(uint32_t win_id)
    {
        window_map()[win_id]->handle_redraw();
    }

    void SDL2_window::handle_window_event(SDL_WindowEvent &ev)
    {
        //std::cout << "Window::handle_window_event()" << std::endl;

        switch (ev.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED: 
            // TODO: is this needed ? difference to SIZE_CHANGE ?
            break;
        case SDL_WINDOWEVENT_RESIZED:
            size_changed(Extents{ev.data1, ev.data2});
            invalidate();
            break;
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_EXPOSED:
            invalidate();
            break;
        case SDL_WINDOWEVENT_CLOSE:
            closing();
            break;
        }
    }

    void SDL2_window::handle_mousemotion_event(SDL_MouseMotionEvent &ev)
    {
        // std::cout << "Window::handle_mousemotion_event()" << std::endl;

        mouse_motion({ev.x, ev.y});
    }

    void SDL2_window::handle_mousebutton_event(SDL_MouseButtonEvent & ev)
    {
        auto dir = ev.state == SDL_PRESSED ? down : up;
        mouse_button(ev.x, ev.y, ev.button, dir, ev.clicks);
    }

    void SDL2_window::handle_mousewheel_event(SDL_MouseWheelEvent & ev)
    {
        // TODO: handle ev.direction
        mouse_wheel(ev.x, ev.y);
    }

    void SDL2_window::handle_textinput_event(SDL_TextInputEvent & ev)
    {
        #if _MSC_VER == 1900
        std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> utf32conv;
        auto utf32 = utf32conv.from_bytes(ev.text);
        text_input(reinterpret_cast<const char32_t *>(utf32.data()), utf32.size());
        #else
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
        auto utf32 = utf32conv.from_bytes(ev.text);
        _text_input_handler(utf32.c_str(), utf32.size());
        p()->text_input(utf32.data(), utf32.size());
        #endif
    }

    void SDL2_window::handle_keydown_event(SDL_KeyboardEvent &ev)
    {
        key_down(ev.keysym); // TODO: pass "repeat" too ?
    }

    void SDL2_window::handle_redraw()
    {
        _must_redraw = true;

    }

    void SDL2_window::for_each_window(std::function<void(SDL2_window*)> callback)
    {
        for (auto& iter: window_map())
        {
            callback(iter.second);
        }
    }

    void SDL2_window::end_of_event_burst()
    {
        if (_must_redraw)
        {
            #ifdef CPPGUI_USING_OPENGL
            SDL_GL_MakeCurrent(_win.get(), _gr_ctx);
            redraw(_gr_ctx);
            #else
            redraw(nullptr); // TODO: a different type of context ?
            #endif
            _must_redraw = false;
        }
    }

    void SDL2_window::invalidate()
    {
        //std::cout << "Window::request_redraw()" << std::endl;

        SDL_Event ev;
        ev.type = _initializer.redraw_window;
        SDL_UserEvent &ue = ev.user;
        ue.windowID = SDL_GetWindowID(_win.get());
        SDL_PushEvent(&ev);
    }

    auto SDL2_window::client_extents() const -> Extents
    {
        Extents ext;

        SDL_GetWindowSize(_win.get(), &ext.w, &ext.h);

        return ext;
    }

    void SDL2_window::Deleter::operator()(SDL_Window * win)
    {
        SDL_DestroyWindow(win);
    }

} // ns cppgui