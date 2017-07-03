#pragma once

#include <memory>
#include <list>
#include "cppgui_config.hpp"
#include CPPGUI_PLATFORM_ADAPTER_HEADER
#include "Canvas.hpp"
#include "Root_widget.hpp"


namespace cppgui {
    
    class Window: public CPPGUI_WINDOW_BASE_CLASS
    {
    public:
        
        class Popup_base: public ISurface
        {
        public:
            explicit Popup_base(ISurface *owner, const Rectangle &);
            ~Popup_base() override;

            void show() override;

            auto rectangle() -> Rectangle override;

            void invalidate() override;

            void add_popup(ISurface *) override;
            void remove_popup(ISurface *) override;

        private:
            Window     *_owner;
            Rectangle   _rect;
        };

        explicit Window(const std::string &title);

        auto& root_widget() { return _root_widget; }

        void add_popup   (ISurface *) override;
        void remove_popup(ISurface *) override;

        void render_ui(Canvas *) override;

    protected:
        
        // Specializing base Window for UI tasks
        void init(void *context) override;
        void cleanup(void *context) override;
        void canvas_created(Canvas *) override;
        void before_destroy_canvas(Canvas *) override;
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

    private:

        using Popup_list = std::list<Popup_base*>;

        void render_canvas(void *context);

        std::unique_ptr<Canvas>     _canvas;
        Root_widget                 _root_widget;

        Popup_list                  _popups;
    };

} // ns cppgui