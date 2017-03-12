#pragma once

#include <stdexcept>
#include <string>
#include <SDL2/SDL_error.h>


class SDL2_exception : public std::exception {
public:
    explicit SDL2_exception(const char *context = nullptr) : msg(SDL_GetError())
    {
        using namespace std::string_literals;
        if (context) msg += " "s + context;
    }
    const char *what() const override { return msg.c_str(); }
private:
    std::string msg;
};
