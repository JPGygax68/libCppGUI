#pragma once

#include <string>
#include <stdexcept>
#include <type_traits>
#include <sstream>
#include "./gl.h"


namespace gl {

    #ifdef OPENGL_IN_GL_NAMESPACE
    using namespace ::gl;
    #endif

    inline void check_previous_call(const char *text, int line, const char *file)
    {
        // Collect error codes (thanks to StackOverflow user Nicol Bolas for pointing out 
        // that there could be more than one) into a string
        bool flag = false;
        std::stringstream error_codes;
        GLenum previous = 0; // GL_NO_ERROR;
        for (GLenum error; (error = glGetError()) != 0; ) // GL_NO_ERROR; )
        {
            if (error == previous) break;
            if (flag) error_codes << ", "; else flag = true;
            error_codes << error;
            previous = error;
        }

        if (flag)
        {
            throw std::runtime_error(std::string(text) 
                + " failed at line " + std::to_string(line)
                + " in file " + file
                + " with OpenGL error code(s) " + error_codes.str()
                );
        }
    }

    // Wrapper for GL calls returning non-void:
    template <typename F, typename ...Args>
    auto call(
        std::enable_if_t<!std::is_void<std::result_of_t<F(Args...)>>::value, const char> *text,
        int line,
        const char *file,
        F && f, Args &&... args
        )
    {
        auto && res = std::forward<F>(f)(std::forward<Args>(args)...);
        check_previous_call(text, line, file);
        return res;
    }

    // Wrapper for GL calls that do not return a value:
    template <typename F, typename ...Args>
    void call(
        std::enable_if_t<std::is_void<std::result_of_t<F(Args...)>>::value, const char> *text,
        int line,
        const char *file,
        F && f, Args &&... args
        )
    {
        std::forward<F>(f)(std::forward<Args>(args)...);
        check_previous_call(text, line, file);
    }

    // The following functions are deprecated

    inline void _throw_error(const char *text, GLenum err, int line, const char *file)
    {
        throw std::runtime_error(std::string(text) +
            ": error " + std::to_string((int)err) +
            " failed at line " + std::to_string(line) +
            " in file " + file
            );
    }

    template<typename F, typename... Args>
    auto func(const char *text, int line, const char *file, F fn, Args&&... args) -> decltype(fn(std::forward<Args>(args)...))
    {
        auto result = fn(std::forward<Args>(args)...);
        //if (result == static_cast<decltype(result)>(0)) throw std::runtime_error(text);
        auto err = glGetError();
        if (err != 0) _throw_error(text, err, line, file);
        return result;
    }

    template<typename P, typename... Args>
    auto proc(const char *text, int line, const char *file, P pr, Args&&... args) -> decltype(pr(std::forward<Args>(args)...))
    {
        pr(std::forward<Args>(args)...);
        auto err = glGetError();
        if (err != 0) _throw_error(text, err, line, file);
    }

} // ns gl
    
#if !defined(GLDEBUG) && defined(_DEBUG)
#define GLDEBUG
#endif

#if defined (GLDEBUG)  

#define CALL_GL(fn, ...) ::gl::func(#fn, __LINE__, __FILE__, fn, __VA_ARGS__)
#define EXEC_GL(pr, ...) ::gl::proc(#pr, __LINE__, __FILE__, pr, __VA_ARGS__)

#define GL(fn, ...) ::gl::call(#fn, __LINE__, __FILE__, gl##fn, __VA_ARGS__)

#else

#define CALL_GL(fn, ...) fn(__VA_ARGS__)
#define EXEC_GL(pr, ...) pr(__VA_ARGS__)

#define GL(cl, ...) gl##cl(__VA_ARGS__)

#endif  

