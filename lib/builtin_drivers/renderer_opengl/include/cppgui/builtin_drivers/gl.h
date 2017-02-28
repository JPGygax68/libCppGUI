#pragma once

#include <cppgui_opengl_config.hpp>

// Include the configured OpenGL bindings

// TODO: except for the type declarations at the end (which should probably be private anyway),
//  it may be better to offer to have CMake define a macro to help import OpenGL, rather than dedicate
//  a whole header file to that purpose. E.g. CPPGUI_OPENGL_HEADER

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
