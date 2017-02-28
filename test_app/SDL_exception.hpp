#pragma once

#include <stdexcept>
#include <string>

#include <SDL_error.h>

class SDL_exception : public std::exception {
public:
    explicit SDL_exception(const char *context = nullptr) : msg(SDL_GetError())
    {
        using namespace std::string_literals;
        if (context) msg += " "s + context;
    }
    const char *what() const override { return msg.c_str(); }
private:
    std::string msg;
};
