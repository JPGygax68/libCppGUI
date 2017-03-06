#version 430

// Viewport width and height
layout(location =  0) uniform int           viewport_w;
layout(location =  1) uniform int           viewport_h;
layout(location =  4) uniform ivec2         position;
layout(location = 10) uniform mat2          texcoord_matrix = mat2(1.0);
layout(location =  5) uniform int           render_mode;
layout(location =  9) uniform ivec4         glyph_cbox;

in  vec2 vp; // vertex position
out vec2 tp; // texel position

void main() {

    // Rendering text glyphs ?
    if (render_mode == 3)
    {
        #ifdef CPPGUI_Y_AXIS_DOWN
        vec2 pixel_position = position + ivec2(vp.x, - vp.y);
        gl_Position = vec4(2 * float(position.x + vp.x) / float(viewport_w) - 1, - (2 * float(position.y + vp.y) / float(viewport_h) - 1), 0.0, 1.0);
        //tp = vec2(vp.x, - vp.y);
        #else
        ivec2 pixel_position = position + ivec2(vp);
        gl_Position = vec4(2 * float(position.x + vp.x) / float(viewport_w) - 1,    2 * float(position.y + vp.y) / float(viewport_h) - 1 , 0.0, 1.0);
        //tp = vp;
        #endif
        tp = vp;
    }
    // Painting color or image ?
    //if (render_mode == 1 || render_mode == 2 || render_mode == 4)
    else
    {
        #ifdef CPPGUI_Y_AXIS_DOWN
        gl_Position = vec4(2 * vp.x / float(viewport_w) - 1, - (2 * vp.y / float(viewport_h) - 1), 0.0, 1.0);
        #else
        gl_Position = vec4(2 * vp.x / float(viewport_w) - 1,    2 * vp.y / float(viewport_h) - 1 , 0.0, 1.0);
        #endif
        tp = texcoord_matrix * (vp.xy - vec2(position));
    }
}
