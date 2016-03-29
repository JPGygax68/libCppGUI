#pragma once

#include <stdexcept>
#include <string>

#include <SDL_error.h>

namespace cppgui {

    namespace sdl {
    
        class Error : public std::exception {
        public:
            explicit Error(const char *context = nullptr) : msg(SDL_GetError())
            {
                using namespace std::string_literals;
                if (context) msg += " "s + context;
            }
            const char *what() const override { return msg.c_str(); }
        private:
            std::string msg;
        };

    } // ns sdl

} // ns cppgui