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
        
        explicit Window(const std::string &title);

        //void set_background_color(const RGBA &);

        auto& root_widget() { return _root_widget; }

    protected:
        
        // Specializing base Window for UI tasks

        void init_window(void *context) override;
        void cleanup_window(void *context) override;
        void redraw(void *context) override;
        void size_changed(const Extents &) override;
        void text_input(const char32_t *text, size_t cp_count) override;
        void key_down(Keycode key) override;
        void mouse_motion(const Point &) override;
        void mouse_button(const Point &p, uint btn, Key_state, uint clicks) override;
        void mouse_wheel(const Point &) override;

        // Further specialization
        virtual void create_ui() {}
        virtual void adjust_layout() {}
        virtual void before_draw_ui(void *context) {}

        // Drawing details (use when overloading redraw())
        void draw_ui();

    private:

        //RGBA                        _bkg_color;
        std::unique_ptr<Canvas>     _canvas;
        Root_widget                 _root_widget;
    };

} // ns cppgui