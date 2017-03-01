/*
The MIT License(MIT)

Copyright(c) 2014 Jean-Pierre Gygax, Biel/Bienne, Switzerland

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <vector>
#include <cassert>
#include <GL/glew.h>
#include <gpc/fonts/rasterized_font.hpp>
#include <OpenGL/error.hpp>

class TextRenderer {
public:

	void setViewportSize(GLint width, GLint height) { vp_width = width, vp_height = height; }

	void setFont(const gpc::fonts::rasterized_font &rfont_) { rfont = &rfont_; }

	void init();

	void drawText(const uint32_t *text, GLint x, GLint y, size_t variant = 0);

	void cleanup();

private:

	void uploadFont();
	void prepareShaderProgram();
	void prepareGlyphVertices();

	GLint vp_width, vp_height;
	const gpc::fonts::rasterized_font *rfont;
	std::vector<GLuint> texture_buffers;
	std::vector<GLuint> buffer_textures;
	GLuint vertex_buffer;
	GLuint shader_program;
	GLuint vertex_shader, fragment_shader;

};