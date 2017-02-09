#pragma once

#include <cppgui_config.hpp>

// Include the configured OpenGL bindings

#ifdef CPPGUI_OPENGL_USE_GLEW
#include <GL/glew.h>
#elif CPPGUI_OPENGL_USE_GLBINDING
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>
#endif

namespace gl
{
    typedef GLfloat FloatVec2[2];
    typedef GLfloat FloatVec3[3];
    typedef GLfloat FloatVec4[4];

    typedef GLint IntVec2[2];
    typedef GLint IntVec3[3];
    typedef GLint IntVec4[4];

    typedef GLfloat Matrix4[4][4];
    typedef GLfloat Matrix3[3][3];
}
