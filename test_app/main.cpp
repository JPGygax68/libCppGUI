#include <iostream>
#include <memory>

#include <SDL_main.h>

//#include <cppgui/sdl/Application.hpp>

#include "./SDL_window.hpp"
#include "./SDL_application.hpp"

int main(int /*argc*/, char * /*argv*/[])
{
    try
    {
        auto win = std::make_unique<SDL_window>("My window");
        SDL_application app;

        app.run();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Press RETURN to terminate" << std::endl;
        char ch; std::cin >> std::noskipws >> ch;
        return -1;
    }
}