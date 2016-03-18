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

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    using Renderer = gpc::gui::gl::renderer<true>;

    struct Widget_config {

        using Renderer = Renderer;
        using Font_handle = Renderer::font_handle;

        using Widget = cppgui::Widget<Widget_config, true>;
        using Label = cppgui::Label<Widget_config, true>;
        using Root_widget = cppgui::Root_widget<Widget_config, true>;
        
        template <class Next_aspects>
        using Widget_updater = cppgui::Default_widget_updater<Widget_config, true>::Aspect<Next_aspects>;
        template <class Next_aspects>
        using Abstract_container_updater = cppgui::Default_abstract_container_updater<Widget_config, true>::Aspect<Next_aspects>;
        template <class Next_aspects>
        using Root_widget_updater = cppgui::Default_root_widget_updater<Widget_config, true>::Aspect<Next_aspects>;
        template <class Next_aspects>
        using Root_widget_container_updater = cppgui::Default_root_widget_container_updater<Widget_config, true>::Aspect<Next_aspects>;

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

    void closing() override;

private:
    Widget_config::Root_widget                 _root_widget;
    Widget_config::Label                       _label;
    Widget_config::Renderer                    *_renderer;
    bool                                        _gfxres_ok = false;
};