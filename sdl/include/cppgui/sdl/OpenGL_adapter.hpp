#pragma once

/*  libCppGUI - A GUI library for C++11/14

Copyright 2016 Hans-Peter Gygax

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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

        /** This is an "aspect": it gets injected into WindowT by way of inheritance - i.e., WindowT must derive
            from either this or another implementation of the RendererAdapter concept.
         */
        template <typename WindowT>
        struct OpenGL_adapter {
    
            class Window_t: public WindowT { friend struct OpenGL_adapter; };

            auto p() { return static_cast<Window_t*>(this); }

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
