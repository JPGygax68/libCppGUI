#version 430

layout(location = 2) uniform int			cbox_x_min;
layout(location = 3) uniform int			cbox_x_max;
layout(location = 4) uniform int			cbox_y_min;
layout(location = 5) uniform int			cbox_y_max;
layout(location = 6) uniform samplerBuffer	font_pixels;
layout(location = 7) uniform int			texture_base;

in vec2 texture_coord;
out vec4 fragment_color;

void main() {

	int col = int(texture_coord.x);
	int row = cbox_y_max - cbox_y_min - int(texture_coord.y) - 1; // texture has top-down scan direction

	float value = texelFetch(font_pixels, texture_base + row * (cbox_x_max - cbox_x_min) + col);

	fragment_color = vec4(value, value, value, 1.0);
}
