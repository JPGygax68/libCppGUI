#version 430

// TODO: renumber uniforms

layout(location =  0) uniform int               viewport_w;
layout(location =  1) uniform int               viewport_h;
layout(location =  2) uniform vec4              color;
layout(location =  3) uniform sampler2DRect     sampler;
layout(location =  5) uniform int               render_mode;
layout(location =  6) uniform ivec2             offset;             // when rendering images: top-left corner inside image
layout(location = 10) uniform mat2              texcoord_matrix = mat2(1.0);
layout(location =  7) uniform samplerBuffer     font_pixels; 
layout(location =  8) uniform int               glyph_base;
layout(location =  9) uniform ivec4             glyph_cbox;

in  vec2 tp;
out vec4 fragment_color;

void main() {

    // Apply single color
    if (render_mode == 1) {

        fragment_color = color;
    }
    // Image pasting
    else if (render_mode == 2) {

        ivec2 tex_size = textureSize(sampler);
        fragment_color = texelFetch(sampler, (ivec2(tp) + offset) % tex_size);
    }
    // Mono image modulating
    // TODO: renumber rendering modes
    else if (render_mode == 4) {

        ivec2 tex_size = textureSize(sampler);
        fragment_color = vec4(color.rgb, color.a * texelFetch(sampler, (ivec2(tp) + offset) % tex_size).a);
    }
    // Glyph rendering
    else if (render_mode == 3) {

        int x_min = glyph_cbox[0], x_max = glyph_cbox[1], y_min = glyph_cbox[2], y_max = glyph_cbox[3];
        int w = x_max - x_min, h = y_max - y_min;

        int col = int(tp.x - x_min);
        #ifdef CPPGUI_Y_AXIS_DOWN
        int row = int(tp.y + y_max);
        #else
        int row = int(y_max - tp.y) - 1;
        #endif

        float alpha = texelFetch(font_pixels, glyph_base + row * w + col).r;

        fragment_color = vec4(color.rgb, alpha * color.a);
    }
}