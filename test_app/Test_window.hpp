#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Resource_mapper.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    using Renderer = gpc::gui::gl::renderer<true>;

    struct Widget_config; // forward declaration so we can do our using's

    using Widget = cppgui::Widget<Widget_config, true>;
    using Label = cppgui::Label<Widget_config, true>;
    using Root_widget = cppgui::Root_widget<Widget_config, true>;
    using Textbox = cppgui::Textbox<Widget_config, true>;

    struct Widget_config {

        using Renderer = Renderer;
        using Font_handle = Renderer::font_handle;

        template <class Aspect_parent>
        using Widget_updater = cppgui::Default_widget_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Abstract_container_updater = cppgui::Default_abstract_container_updater<Widget_config, true>::Aspect<Aspect_parent>;
        //template <class Aspect_parent>
        //using Root_widget_updater = cppgui::Default_root_widget_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Root_widget_container_updater = cppgui::Default_root_widget_container_updater<Widget_config, true>::Aspect<Aspect_parent>;

        //using Container   = cppgui::Container<Widget_config, true>;
        //using Root_widget = cppgui::Root_widget<Widget_config, true>;

        // We override the color mapper
        // TODO: this should not be necessary, the identity mapper should be chosen automatically
        // according to a static boolean set in the renderer class
        using Color_mapper = cppgui::Identity_mapper<Renderer, cppgui::Rgba_norm>;
        using Font_mapper  = cppgui::Default_font_mapper<Renderer>;

    };

    Test_window();

    void init_graphics();
    void cleanup_graphics();

    void redraw();

    void size_changed(int w, int h);
    void mouse_motion(int x, int y);
    void mouse_button(int x, int y, int button, Button_direction dir, int count);

    void closing() override;

private:
    Root_widget                 _root_widget;
    Label                       _label;
    Textbox                     _textbox;
    
    Widget_config::Renderer    *_renderer;
    bool                        _gfxres_ok = false;
};