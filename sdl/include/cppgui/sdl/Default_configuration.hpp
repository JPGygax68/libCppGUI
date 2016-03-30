#pragma once

#include <cppgui/Resource_mapper.hpp>
#include <cppgui/Default_font_mapper.hpp>
#include <cppgui/Widget.hpp>

#include "./Keyboard_adapter.hpp"
#include "./Mouse_adapter.hpp"

namespace cppgui {

    namespace sdl {
    
        /** Defines a default CppGUI configuration struct that will work with SDL.
         */
        template<class RendererT, bool With_layout>
        struct Default_configuration {
    
            using Renderer = typename RendererT;

            using Configuration = typename Default_configuration<Renderer, With_layout>;
            using Font_handle = typename Renderer::font_handle;

            template<class Aspect_parent> using Widget_updater = typename 
                Default_widget_updater<Configuration, With_layout>::template Aspect<Aspect_parent>;

            template<class Aspect_parent> using Abstract_container_Container_updater = typename 
                Default_Abstract_container_Container_updater<Configuration, With_layout>::template Aspect<Aspect_parent>;

            template <class Aspect_parent> using Container_Container_updater = typename 
                Default_Container_Container_updater<Configuration, With_layout>::template Aspect<Aspect_parent>;

            template <class Aspect_parent> using Root_widget_updater = typename 
                Default_Root_widget_Updater<Configuration, With_layout>::template Aspect<Aspect_parent>;

            template <class Aspect_parent> using Root_widget_container_updater = typename 
                Default_Root_widget_Container_updater<Configuration, With_layout>::template Aspect<Aspect_parent>;

            // We override the color mapper
            // TODO: this should not be necessary, the identity mapper should be chosen automatically
            // according to a static boolean set in the renderer class
            using Color_mapper = ::cppgui::Identity_mapper<Renderer, cppgui::Color>;
            using Font_mapper  = ::cppgui::Default_font_mapper<Renderer>;

            using Keyboard = ::cppgui::sdl::Keyboard_adapter;

            using Mouse = ::cppgui::sdl::Mouse_adapter;

        };

    } // ns sdl

} // ns cppgui