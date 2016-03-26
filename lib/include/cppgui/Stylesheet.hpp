#pragma once

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm;
    using Rasterized_font = gpc::fonts::rasterized_font; // TODO: use typedef/struct that encompasses variant, too

    struct Default_stylesheet {


        template <bool hovered, bool focused, bool pressed, bool disabled>
        struct Button {

            static inline auto constexpr face_color() { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; } // light grey

            static inline auto constexpr border_width() { return 1; }
            static inline auto constexpr padding() { return 2; }
        };

        template <bool focused, bool pressed, bool disabled>
        struct Button<true, focused, pressed, disabled>: public Button<false, focused, pressed, disabled> {

            static inline auto constexpr face_color() { return Rgba_norm{ 0.9f, 0.9f, 0.9f, 1 }; }
        };

        struct Text {

            // Selection background color

            template <bool Disabled, bool Focused> struct Selection_background_color;

            template <> struct Selection_background_color<false, false> { 
                static inline constexpr auto value() { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; } };

            template <> struct Selection_background_color<false, true> { 
                static inline constexpr auto value() { return Rgba_norm{ 0.4f, 0.7f, 1, 1 }; } };

            // Caret color
            struct Caret_color { static inline constexpr auto value() { return Rgba_norm{ 0, 0, 0, 0.8f }; } };
        };
    };

    /** Default implementation of the "Styling" aspect.

        This basic implementation does not support changing styles at runtime, and so also does
        not "cascade".

        A more sophisticated implementation might allow overriding values at runtime, and/or
        inheriting values from containers.
     */
    template <class Stylesheet = Default_stylesheet>
    struct Static_styler {

        using Styles = typename Stylesheet;

        static constexpr auto button_face_color(bool hovered, bool focused, bool pressed, bool disabled)
        {
            return (!hovered && !focused && !pressed && !disabled)  ? Styles::Button<false, false, false, false>::face_color() :
                   (!hovered && !focused && !pressed &&  disabled)  ? Styles::Button<false, false, false, true >::face_color() :
                   (!hovered && !focused &&  pressed && !disabled)  ? Styles::Button<false, false, true , false>::face_color() :
                   (!hovered && !focused &&  pressed &&  disabled)  ? Styles::Button<false, false, true , true >::face_color() :
                   (!hovered &&  focused && !pressed && !disabled)  ? Styles::Button<false, true , false, false>::face_color() :
                   (!hovered &&  focused && !pressed &&  disabled)  ? Styles::Button<false, true , false, true >::face_color() :
                   (!hovered &&  focused &&  pressed && !disabled)  ? Styles::Button<false, true , true , false>::face_color() :
                   (!hovered &&  focused &&  pressed &&  disabled)  ? Styles::Button<false, true , true , true >::face_color() :
                   ( hovered && !focused && !pressed && !disabled)  ? Styles::Button<true , false, false, false>::face_color() :
                   ( hovered && !focused && !pressed &&  disabled)  ? Styles::Button<true , false, false, true >::face_color() :
                   ( hovered && !focused &&  pressed && !disabled)  ? Styles::Button<true , false, true , false>::face_color() :
                   ( hovered && !focused &&  pressed &&  disabled)  ? Styles::Button<true , false, true , true >::face_color() :
                   ( hovered &&  focused && !pressed && !disabled)  ? Styles::Button<true , true , false, false>::face_color() :
                   ( hovered &&  focused && !pressed &&  disabled)  ? Styles::Button<true , true , false, true >::face_color() :
                   ( hovered &&  focused &&  pressed && !disabled)  ? Styles::Button<true , true , true , false>::face_color() :
                   ( hovered &&  focused &&  pressed &&  disabled)  ? Styles::Button<true , true , true , true >::face_color() :
                   Rgba_norm{}; // must of course never get there
        }

        static constexpr auto selected_text_background_color(bool /*disabled*/, bool focused) 
        {
            return focused ?
                Styles::Text::Selection_background_color<false, true >::value() :
                Styles::Text::Selection_background_color<false, false>::value();
        }

        static constexpr auto caret_color() { return Styles::Text::Caret_color::value(); }

    };

} // ns cppgui