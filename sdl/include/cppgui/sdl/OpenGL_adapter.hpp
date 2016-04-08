#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <SDL_video.h>

#include <gpc/gl/wrappers.hpp>

namespace cppgui {

    namespace sdl {

        /* Injected into WindowT by way of multiple inheritance - i.e., WindowT is derived from this
         */
        template <typename WindowT>
        struct OpenGL_adapter {
    
            class Window_t: public WindowT { friend struct OpenGL_adapter; };

            auto p() { return static_cast<Window_t*>(this); }

            OpenGL_adapter()
            {
            }

            void init_renderer()
            {
                SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
                _ctx = p()->create_gl_context();
                //if (!_ctx) throw std::runtime_error("cppgui::sdl::OpenGL_adapter::init_renderer(): failed to create OpenGL context");

                p()->make_gl_context_current(_ctx); // by definition, OpenGL context must be active now
                glbinding::Binding::initialize();

                GL(ClearColor, 0.0f, 0.5f, 1.0f, 1.0f);
            }

            void cleanup_renderer() {}

            void make_renderer_context_current()
            {
                p()->make_gl_context_current(_ctx);
            }

            void present()
            {
                p()->gl_swap();
            }

        private:
            SDL_GLContext           _ctx;
        };

    } // ns sdl

} // ns cppgui
