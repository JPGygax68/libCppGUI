#pragma once

#include <memory>
#include "cppgui_config.hpp"
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Root_widget.hpp"


namespace cppgui {
    
    class Window: public CPPGUI_WINDOW_BASE_CLASS
    {
    public:
        
        explicit Window(const char *title);

        void set_background_color(const RGBA &);

        auto& root_widget() { return _root_widget; }

    protected:
        
        // Specializing base Window for UI tasks

        void init_graphics(void *context) override;
        void cleanup_graphics(void *context) override;
        void redraw(void *context) override;
        void size_changed(const Extents &) override;

        void mouse_motion(const Point &) override;

    private:
        RGBA                        _bkg_color;
        std::unique_ptr<Canvas>     _canvas;
        Root_widget                 _root_widget;
    };

} // ns cppgui