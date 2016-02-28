#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Root_widget.hpp>

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    Test_window();

    void init();

    void redraw();

private:
    using Root_widget = cppgui::Root_widget<gpc::gui::gl::renderer<true>>;

    Root_widget _root_widget;
};