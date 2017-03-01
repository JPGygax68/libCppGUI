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

#include <cassert>
#include <memory>
//#include <numeric>

static const unsigned char vertex_source[] = {
#include <TextRenderer_vertex_shader.h>
};

static const unsigned char fragment_source[] = {
#include <TextRenderer_fragment_shader.h>
};

#include "TextRenderer.hpp"

// HELPER ROUTINES --------------------------------------------------

static void loadShader(GLuint shader, const char *source, size_t length)
{
	const char *sources[] = { source };
	const GLint lengths[] = { length };

	GL(glShaderSource, (shader, 1, sources, lengths));
}

static void compileShader(GLuint shader)
{
	GL(glCompileShader, (shader));

	GLint ok;
	GL(glGetShaderiv, (shader, GL_COMPILE_STATUS, &ok));
	
	if (!ok) {
		GLint info_length;
		GL(glGetShaderiv, (shader, GL_INFO_LOG_LENGTH, &info_length));

		auto log = std::unique_ptr<GLchar>(new GLchar[info_length]);
		GL(glGetShaderInfoLog, (shader, info_length, &info_length, log.get()));

		throw std::runtime_error(std::string("Failed to compile the shader: ") + log.get());
	}
}

// METHOD IMPLEMENTATIONS -------------------------------------------

void TextRenderer::init() 
{
	uploadFont();

	prepareGlyphVertices();

	prepareShaderProgram();
}

void TextRenderer::cleanup() 
{
	// TODO: delete the buffers
}

void TextRenderer::uploadFont() 
{
	assert(rfont);

	texture_buffers.resize(rfont->variants.size());
	GL(glGenBuffers, (texture_buffers.size(), &texture_buffers[0]));

	buffer_textures.resize(rfont->variants.size());
	GL(glGenTextures, (buffer_textures.size(), &buffer_textures[0]));

	for (auto i_var = 0U; i_var < rfont->variants.size(); i_var++) {

		GL(glBindBuffer, (GL_TEXTURE_BUFFER, texture_buffers[i_var]));

		auto &variant = rfont->variants[i_var];

		// Load the pixels into a texture buffer object
		GL(glBufferStorage, (GL_TEXTURE_BUFFER, variant.pixels.size(), &variant.pixels[0], 0)); // TODO: really no flags ?

		// Bind the texture buffer object as a.. texture
		GL(glBindTexture, (GL_TEXTURE_BUFFER, buffer_textures[i_var]));
		GL(glTexBuffer, (GL_TEXTURE_BUFFER, GL_R8, texture_buffers[i_var]));
	}

	GL(glBindBuffer, (GL_TEXTURE_BUFFER, 0));
	GL(glBindTexture, (GL_TEXTURE_BUFFER, 0));
}

void TextRenderer::prepareGlyphVertices()
{
	struct Vertex { GLint x, y; };

	GL(glGenBuffers, (1, &vertex_buffer));

	// Compute total number of glyphs in Rasterized Font
	auto glyph_count = 0U;
	for (const auto &variant: rfont->variants) glyph_count += variant.glyphs.size();

	// Allocate a buffer big enough to hold each glyph as a quad or triangle strip
	std::vector<Vertex> vertices;
	vertices.reserve(4 * glyph_count);
	
	// Create a quad of 2D vertices (relative to insertion point) for each glyph
	for (const auto &variant: rfont->variants) {

		// TODO: store and use the base index for each font variant

		for (const auto &glyph : variant.glyphs) {
			/* bottom left  */ vertices.emplace_back<Vertex>({ glyph.cbox.x_min, glyph.cbox.y_min });
			/* bottom right */ vertices.emplace_back<Vertex>({ glyph.cbox.x_max, glyph.cbox.y_min });
			/* top right    */ vertices.emplace_back<Vertex>({ glyph.cbox.x_max, glyph.cbox.y_max });
			/* top left     */ vertices.emplace_back<Vertex>({ glyph.cbox.x_min, glyph.cbox.y_max });
		}
	}

	// Upload the vertex array
	GL(glBindBuffer, (GL_ARRAY_BUFFER, vertex_buffer));
	GL(glBufferData, (GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));
	GL(glBindBuffer, (GL_ARRAY_BUFFER, 0)); // just in case
}

void TextRenderer::prepareShaderProgram()
{
	shader_program = GLR(glCreateProgram, ());

	vertex_shader = GLR(glCreateShader, (GL_VERTEX_SHADER));
	{
		loadShader(vertex_shader, reinterpret_cast<const char *>(vertex_source), sizeof(vertex_source));
		GL(glAttachShader, (shader_program, vertex_shader));
		compileShader(vertex_shader);
	}

	fragment_shader = GLR(glCreateShader, (GL_FRAGMENT_SHADER));
	{
		loadShader(fragment_shader, reinterpret_cast<const char *>(fragment_source), sizeof(fragment_source));
		GL(glAttachShader, (shader_program, fragment_shader));
		compileShader(fragment_shader);
	}

	GL(glLinkProgram, (shader_program));
	GLint length;
	GL(glGetProgramiv, (shader_program, GL_INFO_LOG_LENGTH, &length));
	if (length > 1) {
		auto log = std::unique_ptr<GLchar>(new GLchar[length]);
		GL(glGetProgramInfoLog, (shader_program, length, &length, log.get()));
		throw std::runtime_error(log.get());
	}

	if (!GLR(glIsProgram, (shader_program))) throw std::runtime_error("Shader program is not recognized as a program");
	GL(glValidateProgram, (shader_program));
}

void TextRenderer::drawText(const uint32_t *text, GLint x, GLint y, size_t var_index)
{
	// DEBUGGING CODE, REMOVE
	if (false) {

		GL(glUseProgram, (shader_program));
		GL(glUniform2i, (0, vp_width, vp_height));

		GL(glDisable, (GL_CULL_FACE));
		GL(glBegin, (GL_QUADS));
		GL(glVertex2i, (10, 10));
		GL(glVertex2i, (20, 10));
		GL(glVertex2i, (20, 20));
		GL(glVertex2i, (10, 20));
		GLI(glEnd, ());

		GL(glUseProgram, (0));

		return;
	}

	if (*text == 0) return;

	const auto &variant = rfont->variants[var_index];

	GL(glEnableClientState, (GL_VERTEX_ARRAY));
	GL(glBindBuffer, (GL_ARRAY_BUFFER, vertex_buffer));
	GL(glVertexPointer, (2, GL_INT, 0, 0));

	//GL(glBindBuffer, (GL_TEXTURE_BUFFER, texture_buffers[var_index]));

	GL(glBindTexture, (GL_TEXTURE_BUFFER, buffer_textures[var_index])); // font pixels

	GL(glUseProgram, (shader_program));

	auto glyph_index = rfont->findGlyph(*text);
	const auto &glyph = variant.glyphs[glyph_index];
	x -= glyph.cbox.x_min;

	const uint32_t * p = text;
	
	for (const auto *p = text; *p; p++) {

		auto glyph_index = rfont->findGlyph(*p);
		const auto &glyph = variant.glyphs[glyph_index];

		GL(glUniform2i, (0, vp_width, vp_height));
		GL(glUniform2i, (1, x, y));
		GL(glUniform1i, (2, glyph.cbox.x_min));
		GL(glUniform1i, (3, glyph.cbox.x_max));
        GL(glUniform1i, (4, glyph.cbox.y_min));
        GL(glUniform1i, (5, glyph.cbox.y_max));
        // TODO: bind texture unit 0 to "sampler" (pro forma, default works ok)
		GL(glUniform1i, (7, glyph.pixel_base));

		GLint base = 4 * glyph_index;
		GL(glDrawArrays, (GL_QUADS, base, 4));

		x += glyph.cbox.adv_x;
	}

	GL(glUseProgram, (0));

	GL(glBindTexture, (GL_TEXTURE_BUFFER, 0));

	GL(glDisableClientState, (GL_VERTEX_ARRAY));
	GL(glBindBuffer, (GL_ARRAY_BUFFER, 0));
}