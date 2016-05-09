#pragma once

#include <functional>

#include "./Widget.hpp"
#include "./Icon_resources.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout> struct Checkbox__Layouter {
        template <class Parent_aspect> struct Aspect;
    };

    struct Font_icon_descr;

    template <class Config, bool With_layout>
    class Checkbox: public Checkbox__Layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
    {
    public:
        using Widget_t      = Widget<Config, With_layout>;
        using Canvas_t      = typename Widget_t::Canvas_t;
        //using Font_handle = typename Widget_t::Font_handle;
        using Font_resource = typename Widget_t::Font_resource;

        using State_change_handler = std::function<void(bool)>;

        void set_font(const Rasterized_font *font) { _label_font.assign(font); }
        auto font() const { return _label_font; }
        void set_tick_glyph(const Rasterized_font *, const Font_icon_descr &); // TODO: use improved Icon_resources methods
        void set_label(const std::u32string &label) { _label = label; }
        auto label() const { return _label; }
        bool checked() const { return _checked; }
        void set_checked(bool state) { _checked = state; }

        void on_state_change(State_change_handler handler);

        void init() override;

        void render(Canvas_t *, const Point &offset) override;

        void mouse_click(const Point &, int button, int count) override;

    protected: // for access by Layouter aspect
        
        Font_resource           _label_font;
        Font_resource           _glyph_font;    // TODO: rename to _tick_font ?
        Font_icon_descr         _tick_descr;
        std::u32string          _label;
        State_change_handler    _state_change_handler;

        Point                   _label_pos;
        Point                   _tick_pos;
        Rectangle               _box_rect;

    private:
        bool                    _checked;
    };

    template <class Config>
    struct Checkbox__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent
        {
            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto padding() { return 2; }

            // TODO:
            // void change_font();
            // void change_glyph_font();

        private:
            class Checkbox_t: public Checkbox<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Checkbox_t*>(this); }
            
            void compute_em_bounds();
            void compute_label_size();
            void get_tick_metrics();

            Text_bounding_box       _em_bounds;
            Text_bounding_box       _tick_bounds;
            Extents                 _tick_extents;
            Length                  _box_edge;
            Text_bounding_box       _label_bounds;
        };
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_CHECKBOX(Config, With_layout) \
    template cppgui::Checkbox<Config, With_layout>;
