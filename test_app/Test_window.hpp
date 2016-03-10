#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Frame.hpp>
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
    void mouse_motion(int x, int y);

private:
    //using Frame = cppgui::Frame<Renderer>;
    using Label = cppgui::Label<Renderer, true>;

    cppgui::Frame<Renderer>         _frame;
    Label                           _label;
    Renderer                       *_renderer;
    bool                            _gfxres_ok = false;
};