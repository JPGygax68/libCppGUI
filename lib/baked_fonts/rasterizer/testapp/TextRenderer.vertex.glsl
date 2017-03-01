#version 430

layout(location = 0) uniform ivec2			viewport;
layout(location = 1) uniform ivec2			origin_position;
layout(location = 2) uniform int			cbox_x_min;
layout(location = 3) uniform int			cbox_x_max;
layout(location = 4) uniform int			cbox_y_min;
layout(location = 5) uniform int			cbox_y_max;
layout(location = 6) uniform samplerBuffer	font_pixels;
layout(location = 7) uniform int			texture_base;

in vec2 vertex;
out vec2 texture_coord;

void main() {

	// Vertex coordinates [pixels]
	vec2 pixel_position = origin_position + vertex;

	// Convert to normalized clip coordinates
	gl_Position = vec4(2 * (pixel_position.x / viewport.x - 0.5), 2 * (pixel_position.y / viewport.y - 0.5), 0, 1.0);

	// "Texture" coordinates
	texture_coord = vec2(vertex.x - cbox_x_min, vertex.y - cbox_y_min);
}