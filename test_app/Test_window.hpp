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
    using Renderer = gpc::gui::gl::renderer<true>;

    Test_window();

    void init();
    void cleanup();

    void redraw();

    void size_changed(int w, int h);

private:
    using Root_widget = cppgui::Root_widget<Renderer>;

    Root_widget     _root_widget;
    Renderer       *_renderer;
};