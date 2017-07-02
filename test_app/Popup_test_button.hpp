#pragma once

#include <memory>
#include <cppgui/Text_widget.hpp>
#include <cppgui/Boxed.hpp>
#include <cppgui/Internal_popup.hpp>


namespace cppgui
{
    
    class Popup_test_button_base: public Text_widget
    {
        public:

            //void set_font(const Rasterized_font *);
            //auto font() const { return _font.rasterized; }
            void set_label(const std::u32string &);
            void set_label(const std::string &);
            auto label() const { return _label; }

            void render(Canvas *, const Point &) override;

            bool mouse_click(const Point &pos, int button, Count count) override;
            bool key_down(const Keycode &) override;

            // TODO: visual feedback on mouse down / up

        protected:

            virtual auto label_color(Widget_states) -> RGBA const { return {0, 0, 0, 1}; }

            //Font_resource                           _font;
            RGBA                                    _color = {0, 0, 0, 1};
            std::u32string                          _label;
            Bbox                                    _label_bbox;
            std::unique_ptr<Internal_popup>    _popup_zone;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

        public:
            void init_layout() override;
            auto get_minimal_bbox() -> Bbox override;
            void compute_layout(Bbox_cref) override;

            // Extra capabilities coming with layouting
            // TODO
            // void change_font(const Rasterized_font *);
            // void change_label(const std::u32string &);
            //void set_alignment(Alignment);

            // Extra properties
            //auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

            // Interface with main class (Button)

            void font_changed();
            void text_changed();

        protected:

            void compute_label_bounds();

            void toggle_popup();

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

    using Popup_test_button = Boxed<Popup_test_button_base, Button_box_styles>;

} // ns cppgui
