#pragma once

#include <string>
// TODO: make customizable (other binding libraries) via cppgui_config.hpp
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
//using namespace ::gl;

#include <cppgui/colors.hpp>


namespace cppgui {

    // TODO: rename this class to Renderer_OpenGL and use cppgui_config.in.hpp to define a macro RENDERER_CLASS
    
    class Renderer { 
    public:

        // Metadata

        static const bool font_mapping_is_expensive = true;
        static const bool color_mapping_is_expensive = false;
        //static const bool font_handles_are_resources = true;
        //static const bool colors_are_resources       = false;

    public:

        using rasterized_font = gpc::fonts::rasterized_font;

    public:

        // Exported types

        using offset        = int;
        using length        = int;
        using Image_handle  = GLuint;
        using font_handle   = GLint;
        using native_color  = RGBA;
        using native_mono   = Mono;

        // Class methods

        static constexpr auto rgba_to_native(const RGBA &color) -> RGBA
        {
            return color;
        }

        static constexpr auto mono_to_native(const Mono &color) -> Mono
        {
            return color;
        }

        // Lifecycle

        Renderer();

        void enter_context();

        void leave_context();

        void define_viewport(int x, int y, int width, int height);

        void clear(const RGBA &color);

        auto register_rgba32_image(size_t width, size_t height, const RGBA32 *pixels) -> Image_handle;

        void release_rgba32_image(Image_handle);

        auto register_mono8_image(size_t width, size_t height, const Mono8 *pixels) -> Image_handle; 
            // TODO: use image handle type specific to mono images?

        void release_mono8_image(Image_handle);

        void fill_rect(int x, int y, int w, int h, const RGBA &color);

        // TODO: deprecate and rename to draw_color_image()
        void draw_image(int x, int y, int w, int h, Image_handle image);

        // TODO: deprecate and rename to draw_color_image()
        void draw_image(int x, int y, int w, int h, Image_handle image, int offset_x, int offset_y);

        void modulate_greyscale_image(int x, int y, int w, int h, Image_handle, const RGBA &color, 
            int offset_x = 0, int offset_y = 0);

        /** Collection of image drawing methods intended for drawing an image in the
            4 cardinal directions.
        */

        void draw_greyscale_image_right_righthand(int x, int y, int length, int width, 
            Image_handle, const RGBA &color,  int offset_x = 0, int offset_y = 0);

        void draw_greyscale_image_down_righthand(int x, int y, int length, int width, 
            Image_handle, const RGBA &color,  int offset_x = 0, int offset_y = 0);

        void draw_greyscale_image_left_righthand(int x, int y, int length, int width, 
            Image_handle, const RGBA &color,  int offset_x = 0, int offset_y = 0);

        void draw_greyscale_image_up_righthand(int x, int y, int length, int width, 
            Image_handle, const RGBA &color,  int offset_x = 0, int offset_y = 0);

        void set_clipping_rect(int x, int y, int w, int h);

        void cancel_clipping();

        auto register_font(const rasterized_font &font) -> font_handle;

        void release_font(font_handle reg_font);
        //void release_font(const rasterized_font &);

        void set_text_color(const RGBA &color);

        // auto get_text_extents(reg_font_t font, const char32_t *text, size_t count) -> text_bbox_t;

        void render_text(font_handle font, int x, int y, const char32_t *text, size_t count, int w_max = 0);

        void init();

        void cleanup();

        void draw_rect(int x, int y, int width, int height);

    private:

        void _draw_greyscale_image(int x, int y, int w, int h, Image_handle, const RGBA &color,
            int origin_x, int origin_y, float texrot_sin, float texrot_cos, int offset_x = 0, int offset_y = 0);

        // TODO: move this back into non-template base class

        static constexpr auto vertex_code() -> std::string {

            return std::string {
                #include "generated/renderer_opengl/vertex.glsl.h"
            };
        }

        static constexpr auto fragment_code() -> std::string {

            return std::string {
                #include "generated/renderer_opengl/fragment.glsl.h"
            };
        }

        struct managed_font: gpc::fonts::rasterized_font {
            
            managed_font(const rasterized_font &font_) : gpc::fonts::rasterized_font{ font_ } {}
            
            void store_pixels();
            void create_quads();

            std::vector<GLuint> buffer_textures;
            std::vector<GLuint> textures; // one 1D texture per variant
            GLuint vertex_buffer;
        };

        //static const std::string vertex_code, fragment_code;

        GLuint vertex_buffer, index_buffer;
        GLuint vertex_shader, fragment_shader;
        GLuint program;
        std::vector<GLuint> image_textures;
        std::vector<managed_font> managed_fonts;
        GLint vp_width, vp_height;
        RGBA text_color;

        #ifdef DEBUG
        bool dbg_clipping_active = false;
        #endif
    };
    
} // ns cppguii