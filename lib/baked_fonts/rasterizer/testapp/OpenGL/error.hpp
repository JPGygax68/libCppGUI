#pragma once

#include <sstream>
#include <GL/gl.h>
#include <stdexcept>

namespace gl {

    class Error : public std::runtime_error {
	    static const std::string 
        get_error_string(GLenum err) {
		    std::stringstream ss;
		    ss << err << ": " << (const char *) gluErrorString(err);
		    return ss.str();
	    }
    public:
	    Error(GLenum err, const char * context)
		  : std::runtime_error(
			    std::string("OpenGL error ") +
			    get_error_string(err) + " while executing " + context )
	    {}

        Error(const char *errtxt) 
          : std::runtime_error( std::string(errtxt) )
        {}
    };


#if _DEBUG && (!_NO_OPENGL_CHECKS)

    #define GLREPORT(err, ctx) throw gl::Error(err, ctx)

	#define GLCHECK(ctx) { \
		GLenum err = glGetError(); \
		if (err != GL_NO_ERROR) GLREPORT(err, ctx); \
	}

    // Wrapper for standard OpenGL calls (without a return value):
	#define GL(func, args) { \
		func args; \
		GLCHECK(#func #args); \
	}

    // Use this macro for OpenGL calls with return values:

	#define GLR(func, args) [&]() -> decltype(func args) { \
		auto ret = func args; \
		GLCHECK(#func #args); \
        return ret; \
	} ()

	#define GLI(func, args) { \
		func args; \
		int err = glGetError(); \
	}

#else

	#define GLCHECK(ctx)
	#define GL(func, args) func args
	#define GLI(func, args) func args
	#define GLR(func, args) func args

#endif

} // ns gl
