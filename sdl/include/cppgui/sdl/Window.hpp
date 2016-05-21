#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include <iostream>

#include <SDL.h>
#include <SDL_events.h>

#include "./error_handling.hpp"
#include "./Application.hpp"

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

            Window(const std::string &title, int w = 800, int h = 600) // TODO: should probably be made protected
            {
                global().init();

                auto win = SDL_CreateWindow(title.c_str(),
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
                    /* SDL_WINDOW_FULLSCREEN_DESKTOP | */ SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

                _win.reset(win);

                window_map()[id()] = static_cast<Impl*>(this);
            }

            ~Window()
            {
                window_map().erase(id());
            }

            auto id() -> uint32_t
            {
                return SDL_GetWindowID(_win.get());
            }

            // OpenGL
            // TODO: move to renderer aspect

            auto get_current_gl_context() -> SDL_GLContext
            {
                return SDL_GL_GetCurrentContext();
            }
            auto create_gl_context() -> SDL_GLContext
            {
                SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
                auto ctx = SDL_GL_CreateContext(_win.get());
                if (!ctx) throw Error("trying to create OpenGL context for a window");
                return ctx;
            }
            static void delete_gl_context(SDL_GLContext ctx)
            {
                SDL_GL_DeleteContext(ctx);
            }
            void make_gl_context_current(SDL_GLContext ctx)
            {
                //if (SDL_GL_MakeCurrent(_win.get(), ctx) < 0) throw sdl::Error("trying to make GL context current");
                int err;
                int count;
                for (count = 0; ;)
                {
                    count ++;
                    err = SDL_GL_MakeCurrent(_win.get(), ctx);
                    if (err == 0) break;
                    if (count >= 10) throw sdl::Error("trying to make GL context current (repeated attempts to call SDL_GL_MakeCurrent())");
                    SDL_Delay(1);
                }
                if (count > 1) std::cerr << "cppgui::sdl::Window<>::make_gl_context_current() has required " << count << " attempts" << std::endl;
            }
            void gl_swap()
            {
                SDL_GL_SwapWindow(_win.get());
            }

            void invalidate()
            {
                //std::cout << "Window::request_redraw()" << std::endl;

                SDL_Event ev;
                ev.type = global().redraw_window;
                SDL_UserEvent &ue = ev.user;
                ue.windowID = SDL_GetWindowID(_win.get());
                SDL_PushEvent(&ev);
            }

            auto client_extents() const -> Extents
            {
                Extents ext;

                SDL_GetWindowSize(_win.get(), &ext.w, &ext.h);

                return ext;
            }

            //virtual void closing() {};

            static void dispatch_window_event(SDL_WindowEvent &ev)
            {
                window_map()[ev.windowID]->handle_window_event(ev);
            }
            static void dispatch_mousemotion_event(SDL_MouseMotionEvent &ev)
            {
                window_map()[ev.windowID]->handle_mousemotion_event(ev);
            }
            static void dispatch_mousebutton_event(SDL_MouseButtonEvent &ev)
            {
                window_map()[ev.windowID]->handle_mousebutton_event(ev);
            }
            static void dispatch_mousewheel_event(SDL_MouseWheelEvent &ev)
            {
                window_map()[ev.windowID]->handle_mousewheel_event(ev);
            }
            static void dispatch_textinput_event(SDL_TextInputEvent &ev)
            {
                window_map()[ev.windowID]->handle_textinput_event(ev);
            }
            static void dispatch_keydown_event(SDL_KeyboardEvent &ev)
            {
                window_map()[ev.windowID]->handle_keydown_event(ev);
            }
            static void dispatch_redraw(uint32_t win_id)
            {
                window_map()[win_id]->handle_redraw();
            }
            static void dispatch_custom_event(uint32_t win_id)
            {
                window_map()[win_id]->handle_redraw();
            }

            // Interface towards Application<>

            static void for_each_window(std::function<void(Impl*)> callback)
            {
                for (auto& iter: window_map())
                {
                    callback(iter.second);
                }
            }

            void end_of_event_burst()
            {
                if (_must_redraw)
                {
                    p()->make_renderer_context_current();
                    p()->redraw();
                    _must_redraw = false;
                }
            }

        protected:

            void init_window() // use CRTP-override
            {
                p()->init_renderer(); // call into renderer adapter
                                      // TODO: other subsystem adapters
            }                                      
            void cleanup_window()
            {
                p()->cleanup_renderer();
                // TODO: other subsystem adapters
            }

            void handle_window_event(SDL_WindowEvent &ev)
            {
                //std::cout << "Window::handle_window_event()" << std::endl;

                switch (ev.event)
                {
                case SDL_WINDOWEVENT_SIZE_CHANGED: 
                    // TODO: is this needed ? difference to SIZE_CHANGE ?
                    break;
                case SDL_WINDOWEVENT_RESIZED:
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
            void handle_mousemotion_event(SDL_MouseMotionEvent &ev)
            {
                // std::cout << "Window::handle_mousemotion_event()" << std::endl;

                p()->mouse_motion(ev.x, ev.y);
            }
            void handle_mousebutton_event(SDL_MouseButtonEvent &ev)
            {
                auto dir = ev.state == SDL_PRESSED ? down : up;
                p()->mouse_button(ev.x, ev.y, ev.button, dir, ev.clicks);
            }
            void handle_mousewheel_event(SDL_MouseWheelEvent &ev)
            {
                // TODO: handle ev.direction
                p()->mouse_wheel(ev.x, ev.y);
            }
            void handle_textinput_event(SDL_TextInputEvent &ev)
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
            void handle_keydown_event(SDL_KeyboardEvent &ev)
            {
                p()->key_down(ev.keysym); // TODO: pass "repeat" too ?
            }
            void handle_redraw()
            {
                _must_redraw = true;
            }
            virtual void handle_custom_event(SDL_UserEvent &) {}

        private:
            class Window_t: public Impl { friend class Window; };
            auto p() { return static_cast<Window_t*>(this); }

            Window(const Window &) = delete; // no copy constructor please
            struct Deleter { void operator() (SDL_Window *win) { SDL_DestroyWindow(win); } };
            
            struct Global_data 
            {
                uint32_t        redraw_window;

                void init()
                {
                    //SDL_Init(SDL_INIT_EVERYTHING); //SDL_INIT_VIDEO);
                    if (SDL_VideoInit(nullptr) < 0) throw std::runtime_error("trying to initialize SDL video subsystem");
                    atexit(&SDL_VideoQuit);

                    redraw_window = Application<Impl>::register_custom_event([](const SDL_UserEvent &ev) {

                        Window<Impl>::dispatch_redraw(ev.windowID);
                    });

                }
            };

            static auto global() -> Global_data &
            {
                static struct Global_data data {};
                static bool initialized = false;

                if (!initialized)
                {
                    data.init();
                    initialized = true;
                }

                return data;
            }

            static auto window_map() -> std::map<uint32_t, Impl*> &
            {
                static std::map<uint32_t, Impl*> map;

                return map;
            }

            Pointer         _win;
            bool            _must_redraw;
        };

    } // ns sdl

} // ns cppgui