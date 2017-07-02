#pragma once

#include <memory>
#include <cppgui/Text_widget.hpp>
#include <cppgui/Boxed.hpp>
#include <cppgui/Window.hpp>


namespace cppgui
{
    
    class Popup_test_button_base: public Text_widget
    {
        public:

            explicit Popup_test_button_base(const std::u32string &label);

            void set_label(const std::u32string &);
            void set_label(const std::string &);
            auto label() const { return _label; }

            void render(Canvas *, const Point &) override;

            bool mouse_click(const Point &pos, int button, Count count) override;
            bool key_down(const Keycode &) override;

            // TODO: visual feedback on mouse down / up

        protected:

            virtual auto label_color(Widget_states) -> RGBA const { return {0, 0, 0, 1}; }
            void toggle_popup();

            RGBA                                    _color = {0, 0, 0, 1};
            std::u32string                          _label;
            Bbox                                    _label_bbox;
            std::unique_ptr<Window::Popup_base>     _popup;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

        public:
            void init_layout() override;
            auto get_minimal_bbox() -> Bbox override;
            void compute_layout(Bbox_cref) override;

            void font_changed();
            void text_changed();

        protected:

            void compute_label_bounds();

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

    using Popup_test_button = Boxed<Popup_test_button_base, Button_box_styles>;

} // ns cppgui
