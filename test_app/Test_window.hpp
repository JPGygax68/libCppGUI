#pragma once

#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/all_widgets.hpp>
#include <cppgui/GUI_window.hpp>

#include <cppgui/sdl/Window.hpp>
#include <cppgui/sdl/Default_configuration.hpp>
#include <cppgui/sdl/OpenGL_adapter.hpp>

using Renderer = typename gpc::gui::gl::renderer<true>;
struct GUI_configuration: cppgui::sdl::Default_configuration<GUI_configuration, Renderer, true> {

    using Default_font = typename cppgui::Default_font<16>;
};

class Test_window;
using My_SDL_window = cppgui::sdl::Window<Test_window>;
using My_GUI_window = cppgui::GUI_window<GUI_configuration, My_SDL_window, cppgui::sdl::OpenGL_adapter>;

class Test_window: public My_GUI_window {
public:
    using Parent = My_GUI_window;
    CPPGUI_DEFINE_WIDGET_TYPES(GUI_configuration, true);

    Test_window();

private:
    Label                       _label;
    Button                      _button;
    Textbox                     _textbox;
    Checkbox                    _checkbox;
    std::vector<Button>         _button_list;
    Stack                       _stack;
    Stack                       _menu;
};
