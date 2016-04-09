#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>

#include "error_handling.hpp"

struct SDL_Window;
struct SDL_WindowEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;
struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;
struct SDL_Keysym;
using SDL_GLContext = void *;

namespace cppgui {

    namespace sdl {

        // TODO: split into base class and specialization for OpenGL

        /** Impl = implementation (compile-time inheritance via CRTP)

            Impl must provide:
            
            redraw()        must render the whole content
                            TODO: optimizations to render only changed parts
         */
        template <class Impl>
        class Window 
        {
            struct Deleter;

        public:
            using Pointer = std::unique_ptr<SDL_Window, Deleter>;
            struct Extents { int w, h; };
            enum Button_direction { down = 0, up = 1 };

            Window(const std::string &title, int w = 800, int h = 600); // TODO: should probably be made protected
            ~Window();

            auto id() -> uint32_t;

            // OpenGL

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
            //static void dispatch_custom_event(uint32_t win_id);

        protected:

            void init_window(); // use CRTP-override
            void cleanup_window(); // ditto

            void handle_window_event(SDL_WindowEvent &ev);
            void handle_mousemotion_event(SDL_MouseMotionEvent &ev);
            void handle_mousebutton_event(SDL_MouseButtonEvent &ev);
            void handle_mousewheel_event(SDL_MouseWheelEvent &ev);
            void handle_textinput_event(SDL_TextInputEvent &ev);
            void handle_keydown_event(SDL_KeyboardEvent &ev);
            void handle_redraw();
            //void handle_custom_event(SDL_UserEvent &ev);

        private:
            class Window_t: public Impl { friend class Window; };
            auto p() { return static_cast<Window_t*>(this); }

            Window(const Window &) = delete; // no copy constructor please
            struct Deleter { void operator() (SDL_Window *win); };
            
            struct Static_init;
            static Static_init _initializer;

            static auto window_map() -> std::map<uint32_t, Impl*> &;

            Pointer         _win;
        };

    } // ns sdl

} // ns cppgui