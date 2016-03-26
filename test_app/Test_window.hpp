#pragma once

#pragma warning(push)
#pragma warning(disable:4251)
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#pragma warning(pop)
using namespace gl;

#include <gpc/gui/gl/renderer.hpp> // TODO: replace with new namespace/naming
#include <cppgui/Resource_mapper.hpp>
#include <cppgui/Default_font_mapper.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Stack.hpp>

#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    using Renderer = gpc::gui::gl::renderer<true>;

    struct Widget_config; // forward declaration so we can do our using's

    using Widget = cppgui::Widget<Widget_config, true>;
    using Label = cppgui::Label<Widget_config, true>;
    using Button = cppgui::Button<Widget_config, true>;
    using Root_widget = cppgui::Root_widget<Widget_config, true>;
    using Textbox = cppgui::Textbox<Widget_config, true>;
    using Stack = cppgui::Stack<Widget_config, true>;

    struct Widget_config {

        using Renderer = Renderer;
        using Font_handle = Renderer::font_handle;

        template <class Aspect_parent>
        using Widget_updater = cppgui::Default_widget_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Abstract_container_updater = cppgui::Default_abstract_container_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Container_Container_updater = cppgui::Default_Container_Container_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Root_widget_updater = cppgui::Default_root_widget_updater<Widget_config, true>::Aspect<Aspect_parent>;
        template <class Aspect_parent>
        using Root_widget_container_updater = cppgui::Default_root_widget_container_updater<Widget_config, true>::Aspect<Aspect_parent>;

        //using Container   = cppgui::Container<Widget_config, true>;
        //using Root_widget = cppgui::Root_widget<Widget_config, true>;

        // We override the color mapper
        // TODO: this should not be necessary, the identity mapper should be chosen automatically
        // according to a static boolean set in the renderer class
        using Color_mapper = cppgui::Identity_mapper<Renderer, cppgui::Color>;
        using Font_mapper  = cppgui::Default_font_mapper<Renderer>;

        //using Styler = cppgui::Static_styler<cppgui::Default_stylesheet>;

        // TODO: move this to a submodule of cppgui
        struct Keyboard {

            using Keycode = SDL_Keysym;

            static inline bool is_left     (const Keycode &key) { return key.sym == SDLK_LEFT     ; }
            static inline bool is_right    (const Keycode &key) { return key.sym == SDLK_RIGHT    ; }
            static inline bool is_backspace(const Keycode &key) { return key.sym == SDLK_BACKSPACE; }
            static inline bool is_delete   (const Keycode &key) { return key.sym == SDLK_DELETE   ; }
            static inline bool is_home     (const Keycode &key) { return key.sym == SDLK_HOME     ; }
            static inline bool is_end      (const Keycode &key) { return key.sym == SDLK_END      ; }

            // Modifiers 

            static inline bool is_shift_down() { return (SDL_GetModState() & KMOD_SHIFT) != 0; }
            static inline bool is_ctrl_down () { return (SDL_GetModState() & KMOD_CTRL ) != 0; }
        };

        struct Mouse {

            static inline bool is_button_down(int button) { return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button)) != 0; }
        };

        struct Cursor {

            using Cursor_handle = SDL_Cursor *;

            static auto get_current_cursor() {

                return SDL_GetCursor();
            }

            static auto get_arrow_cursor() {

                // TODO: do we have to manage this resource ?
                return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            }

            static auto get_ibeam_cursor() {

                // TODO: do we have to manage this resource ?
                return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
            }

            static void free_cursor(Cursor_handle cursor) {

                SDL_FreeCursor(cursor);
            }

            static void set_cursor(Cursor_handle cursor) {

                SDL_SetCursor(cursor);
            }

        };
    };

    Test_window();

    void init_graphics();
    void cleanup_graphics();

    void redraw();

    void size_changed(int w, int h);
    void mouse_motion(int x, int y);
    void mouse_button(int x, int y, int button, Button_direction dir, int count);
    void text_input(const char32_t *, size_t);
    void key_down(const SDL_Keysym &);

    void closing() override;

private:
    Root_widget                 _root_widget;
    Label                       _label;
    Button                      _button;
    Textbox                     _textbox;
    std::vector<Button>         _button_list;
    Stack                       _stack;
    
    Widget_config::Renderer    *_renderer;
    //bool                        _gfxres_ok = false;
};