#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Resource_mapper.hpp>
#include <cppgui/Default_font_mapper.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Stack.hpp>
#include <cppgui/Canvas.hpp>
#include <cppgui/GUI_window.hpp>

#include <cppgui/sdl/Window.hpp>
#include <cppgui/sdl/Default_configuration.hpp>
#include <cppgui/sdl/OpenGL_adapter.hpp>

using Renderer = typename gpc::gui::gl::renderer<true>;
using GUI_configuration = typename cppgui::sdl::Default_configuration<Renderer, true>;

class Test_window: public cppgui::GUI_window<GUI_configuration, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>
{
public:
    using Parent = typename cppgui::GUI_window<GUI_configuration, cppgui::sdl::Window<Test_window>, cppgui::sdl::OpenGL_adapter>;
    using Renderer = typename gpc::gui::gl::renderer<true>;

    using Widget_config = typename cppgui::sdl::Default_configuration<Renderer, true>;

    using Widget = cppgui::Widget<Widget_config, true>;
    using Label = cppgui::Label<Widget_config, true>;
    using Button = cppgui::Button<Widget_config, true>;
    using Root_widget = cppgui::Root_widget<Widget_config, true>;
    using Textbox = cppgui::Textbox<Widget_config, true>;
    using Stack = cppgui::Stack<Widget_config, true>;
    using Canvas_t = cppgui::Canvas<Renderer>;

    Test_window();

private:
    //Root_widget                 _root_widget;
    Label                       _label;
    Button                      _button;
    Textbox                     _textbox;
    std::vector<Button>         _button_list;
    Stack                       _stack;
};