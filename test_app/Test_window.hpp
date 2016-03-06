#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Label.hpp>

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    using Renderer = gpc::gui::gl::renderer<true>;

    Test_window();

    void init_graphics();
    void cleanup_graphics();

    void redraw();

    void size_changed(int w, int h);

private:
    //using Root_widget = cppgui::Root_widget<Renderer>;
    using Label = cppgui::Label<Renderer>;

    Label           _label;
    Renderer       *_renderer;
    bool            _gfxres_ok = false;
};