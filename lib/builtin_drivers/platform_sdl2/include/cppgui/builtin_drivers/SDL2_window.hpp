#pragma once

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>
#include <functional>

#include <SDL.h>

#include <cppgui_config.hpp>
#include <cppgui/basic_types.hpp>

#include "platform_sdl2.hpp"


struct SDL_Window;
struct SDL_WindowEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;
struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;
struct SDL_UserEvent;
struct SDL_Keysym;
using SDL_GLContext = void *;

// TODO: split into base class and specialization for OpenGL


namespace cppgui
{    
    // Derive from this, NOT directly from SDL2_window
    #define CPPGUI_WINDOW_BASE_CLASS    SDL2_window

    class SDL2_window 
    {
        struct Deleter;

    public:
        using Pointer = std::unique_ptr<SDL_Window, Deleter>;
        enum Button_direction { down = 0, up = 1 };

        // Lifecycle --------------------------------------

        explicit SDL2_window(const std::string &title, int w = 800, int h = 600); // TODO: should probably be made protected
        virtual ~SDL2_window();

        /* TODO: the following lifecycle methods are only required because a C++ constructor
         * cannot call a specialized virtual method. CRTP specialization could solve that
         * problem. A decision needs to be made regarding this.
         * An alternative could be to post a user-defined message that will serve as the
         * "created" event.
         */
        void init();
        void cleanup();

        auto id() -> uint32_t;

        // Rendering management ---------------------------

        void begin_rendering();
        void done_rendering();

        // OpenGL-specific --------------------------------

        auto get_current_gl_context() -> SDL_GLContext;
        auto create_gl_context() -> SDL_GLContext;           
        static void delete_gl_context(SDL_GLContext);
        void make_gl_context_current(SDL_GLContext);
        void gl_swap();

        void invalidate();

        auto client_extents() const -> Extents;

        //virtual void closing() {};

        static void dispatch_window_event(SDL_WindowEvent &ev);
        static void dispatch_mousemotion_event(SDL_MouseMotionEvent &ev);
        static void dispatch_mousebutton_event(SDL_MouseButtonEvent &ev);
        static void dispatch_mousewheel_event(SDL_MouseWheelEvent &ev);
        static void dispatch_textinput_event(SDL_TextInputEvent &ev);
        static void dispatch_keydown_event(SDL_KeyboardEvent &ev);
        static void dispatch_redraw(uint32_t win_id);
        static void dispatch_custom_event(uint32_t win_id);

        // Interface towards Application<>

        static void for_each_window(std::function<void(SDL2_window*)>);

        void end_of_event_burst();

    protected:

        // Specialization
        virtual void init_window(void *context) = 0;
        virtual void cleanup_window(void *context) = 0;
        virtual void redraw(void *context) = 0;

        // Lifecycle hooks
        virtual void closing() {}

        // Event hooks
        virtual void size_changed(const Extents &) {}
        virtual void mouse_motion(const Point &) {}
        virtual void mouse_button(int x, int y, uint8_t button, Button_direction dir, unsigned int clicks) {}
        virtual void mouse_wheel(int x, int y) {}
        virtual void text_input(const char32_t *text, size_t cp_count) {}
        virtual void key_down(Keycode key) {}

    private:
        // Event handling
        void handle_window_event     (SDL_WindowEvent      &ev);
        void handle_mousemotion_event(SDL_MouseMotionEvent &ev);
        void handle_mousebutton_event(SDL_MouseButtonEvent &ev);
        void handle_mousewheel_event (SDL_MouseWheelEvent  &ev);
        void handle_textinput_event  (SDL_TextInputEvent   &ev);
        void handle_keydown_event    (SDL_KeyboardEvent    &ev);
        void handle_redraw();
        // /*virtual*/ void handle_custom_event(SDL_UserEvent &) {}

        SDL2_window(const SDL2_window &) = delete; // no copy constructor please
        struct Deleter { void operator() (SDL_Window *win); };

        //struct Static_init;
        //static Static_init _initializer;

        static auto window_map() -> std::map<uint32_t, SDL2_window*> &;

        Pointer         _win;
        #ifdef CPPGUI_USING_OPENGL
        SDL_GLContext   _gr_ctx;
        #else
        #error No or unsupported graphics backend 
        #endif

        bool            _must_redraw;
    };

} // ns cppgui