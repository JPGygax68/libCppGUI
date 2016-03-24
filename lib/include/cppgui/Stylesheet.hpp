#pragma once

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm;
    using Rasterized_font = gpc::fonts::rasterized_font; // TODO: use typedef/struct that encompasses variant, too

    struct Default_stylesheet {

        struct Button {

            template <bool hovered, bool pressed, bool disabled>
            struct Face_color {};

            template <bool pressed>
            struct Face_color<false, pressed, false> { inline constexpr auto value() const { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; } };
        };

        struct Text {

            template <bool Disabled, bool Focused>
            struct Selection_background_color;

            template <>
            struct Selection_background_color<false, false> { static inline constexpr auto value() { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; } };

            template <>
            struct Selection_background_color<false, true> { static inline constexpr auto value() { return Rgba_norm{ 0.4f, 0.7f, 1, 1 }; } };

            struct Caret_color { static inline constexpr auto value() { return Rgba_norm{ 0, 0, 0, 0.8f }; } };
        };
    };

    /** Default implementation of the "Styling" aspect.

        This basic implementation does not support changing styles at runtime.
     */
    template <class Stylesheet = Default_stylesheet>
    struct Static_styler {

        using Styles = typename Stylesheet;

        struct Text {

            static constexpr auto selected_text_background_color(bool /*disabled*/, bool focused) 
            {
                return focused ?
                    Styles::Text::Selection_background_color<false, true >::value() :
                    Styles::Text::Selection_background_color<false, false>::value();
            }

            static constexpr auto caret_color() { return Styles::Text::Caret_color::value(); }
        };

    };

} // ns cppgui