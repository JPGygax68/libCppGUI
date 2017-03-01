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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdexcept>
#include <sstream>

#include <GL/glew.h>
#include <GL/GLU.h>

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_opengl.h>

#include <cereal/archives/binary.hpp>
#include <gpc/fonts/cereal.hpp>

#include <OpenGL/error.hpp>

#include "TextRenderer.hpp"

static const unsigned char embedded_font[] = {
#include "embedded_font.h"
};

static TextRenderer text_renderer;

void initGL()
{
	/* Enable smooth shading */
	GL(glShadeModel, (GL_SMOOTH));

	/* Set the background black */
	GL(glClearColor, (0.0f, 0.0f, 0.0f, 0.0f));

	/* Depth buffer setup */
	GL(glClearDepth, (1.0f));

	/* Enables Depth Testing */
	GL(glEnable, (GL_DEPTH_TEST));

	/* The Type Of Depth Test To Do */
	GL(glDepthFunc, (GL_LEQUAL));

	/* Really Nice Perspective Calculations */
	GL(glHint, (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST));

	glewInit();

	// Initialize the Text Renderer
	text_renderer.init();
}

/* function to reset our viewport after a window resize
 */
int setViewport(int width, int height)
{
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height == 0) height = 1;

	ratio = (GLfloat)width / (GLfloat)height;

	/* Setup our viewport. */
	GL(glViewport, (0, 0, (GLsizei)width, (GLsizei)height));

	/* change to the projection matrix and set our viewing volume. */
	GL(glMatrixMode, (GL_PROJECTION));
	GL(glLoadIdentity, ());

	/* Set our perspective */
	//GL(gluPerspective, (45.0f, ratio, 0.1f, 100.0f));
	GL(glOrtho, (0, (GLdouble)width, 0, (GLdouble)height, -1, 1));

	/* Make sure we're chaning the model view and not the projection */
	GL(glMatrixMode, (GL_MODELVIEW));

	/* Reset The View */
	GL(glLoadIdentity, ());

	text_renderer.setViewportSize(width, height);

	return 1;
}

void render(SDL_Window *window)
{
    /* Set the background black */
    GL(glClearColor, (0.0f, 0.0f, 0.0f, 0.0f));
	/* Clear The Screen And The Depth Buffer */
	GL(glClear, (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	/* Draw text */
	const uint32_t text[] = { 'A', 'B', 'C', 'D', 'g', 'i', ',', 0 };
	text_renderer.drawText(text, 0, 4);

	SDL_GL_SwapWindow(window);
}


int
main(int argc, char *argv[])
{
	try {

		gpc::fonts::rasterized_font rfont;
		{
			std::stringstream is(std::string(reinterpret_cast<const char *>(embedded_font), sizeof(embedded_font)), std::ios_base::in);
			cereal::BinaryInputArchive archive(is);
			archive(rfont);
		}
		text_renderer.setFont(rfont);

		SDL_Init(SDL_INIT_VIDEO);

		SDL_Window *window = SDL_CreateWindow("GPC Font Rasterizer Test App",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600, SDL_WINDOW_OPENGL);
		if (!window) throw std::runtime_error("Failed to open window");

		SDL_GLContext glctx = SDL_GL_CreateContext(window);
		if (!glctx) throw std::runtime_error("Failed to create an OpenGL context for the window");

		SDL_GL_MakeCurrent(window, glctx);

		initGL();

		setViewport(800, 600);

		render(window);

		SDL_Event event;

		while (SDL_WaitEvent(&event)) {

			bool exit = false, must_render = false;

			switch (event.type){

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == 27)
					exit = true;
				break;

			case SDL_KEYUP:
				break;

			case SDL_QUIT:
				exit = true;
				break;

			default:
				break;
			}

			if (exit) break;

			if (must_render) {
				render(window);
				must_render = false;
			}
		}

		SDL_Quit();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}