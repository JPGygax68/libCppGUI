#pragma once

//#include <cppgui/Resource_mapper.hpp>
//#include <cppgui/Default_font_mapper.hpp>
#include <cppgui/Widget.hpp>
#include <cppgui/Container_base.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Default_font.hpp>

#include "./Keyboard_adapter.hpp"
#include "./Mouse_adapter.hpp"

// Forward declaration
// TODO: does not belong here
template<std::size_t Size> struct liberationsans_regular; // { static auto get() -> std::pair<const uint8_t *, std::size_t>; };

namespace cppgui {

    namespace sdl {
    
        /** Defines a default CppGUI configuration struct that will work with SDL.
         */
        template<class ConfigT, class RendererT, bool With_layout>
        struct Default_configuration
        {    
            using Renderer = typename RendererT;

            using Configuration = typename ConfigT; // Default_configuration<Renderer, With_layout>;
            using Font_handle = typename Renderer::font_handle;

            template<class Parent> using Widget_updater =  
                Default__Widget__Updater<Configuration, With_layout, Parent>;

            template<class Class, class Parent> 
            using Abstract_container_Container_updater = typename 
                Default_Abstract_container_Container_updater<Configuration, With_layout>::template Aspect<Class, Parent>;

            template <class Class, class Parent> 
            using Container_base__Container_updater = 
                Default__Container_base__Container_updater<Class, Configuration, With_layout, Parent>; // TODO: normalized form ?

            template <class Parent> using Root_widget__Updater =  
                Default__Root_widget__Updater<Configuration, With_layout, Parent>;

            template <class Class, class Parent>
            using Root_widget__Container_updater =
                Default__Root_widget__Container_updater<Class, Configuration, With_layout, Parent>;

            // We override the color mapper
            // TODO: this should not be necessary, the identity mapper should be chosen automatically
            // according to a static boolean set in the renderer class
            //using Color_mapper = ::cppgui::Identity_mapper<Renderer, cppgui::Color>;
            //using Font_mapper  = ::cppgui::Default_font_mapper<Renderer>;

            using Keyboard = ::cppgui::sdl::Keyboard_adapter;

            using Mouse = ::cppgui::sdl::Mouse_adapter;

            // Default font

            //template <Font_size Size>  using Default_font = typename cppgui::Default_font<Size>;
            using Default_font = typename cppgui::Default_font<>;
        };

    } // ns sdl

} // ns cppgui