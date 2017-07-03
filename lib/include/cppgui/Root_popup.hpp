#pragma once

#include "Root_widget.hpp"


namespace cppgui
{
    
    template<class Base>
    class Root_popup: public Base
    {
    public:
        
        Root_popup(ISurface *surface, const Rectangle &rect): 
            Base{surface, rect},
            _root_widget{this}
        {
        }

        void init(void *context) override
        {
            // Nothing to do here
        }

        void cleanup(void *context) override
        {
            // Nothing to do here
        }

        // TODO: call this!! but when, and from where ?

        void canvas_created(Canvas *canvas) override
        {
            _root_widget.get_backend_resources(canvas);
        }

        void before_destroy_canvas(Canvas *canvas) override
        {
            //_root_widget.release_backend_resources(canvas);
        }

        void render_ui(Canvas *canvas) override
        {
            _root_widget.render(canvas, {0, 0});
        }

        void size_changed(const Extents &ext) override
        {
            // TODO: implement this in an intermediary base class ?
            // (Re-)do layout
            _root_widget.init_layout(); // TODO: should not be called more than once
            _root_widget.set_bounds({0, 0}, Bbox{ext, left, top});
            _root_widget.compute_view_from_data();  // TODO: clarify if this may be called more than once
            this->invalidate();
        }
        
        void text_input(const char32_t *text, size_t cp_count) override
        {
            assert(false);
        }
        
        void key_down(Keycode key) override
        {
            assert(false);
        }
        
        void mouse_motion(const Point &v) override
        {
            _root_widget.mouse_motion(v);
        }
        
        void mouse_button(const Point &p, uint btn, Key_state state, uint clicks) override
        {
            _root_widget.mouse_button(p, btn, state, clicks);
        }

        void mouse_wheel(const Point &v) override
        {
            _root_widget.mouse_wheel(v);
        }

    protected:

        auto& root_widget() { return _root_widget; }

    private:
        Root_widget             _root_widget;
    };

} // ns cppgui
