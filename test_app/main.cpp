#include <iostream>

#include <SDL_main.h>

#include <cppgui/sdl/Application.hpp>

#include "./Main_window.hpp"

int main(int /*argc*/, char * /*argv*/[])
{
    try
    {
        auto win = std::make_unique<Main_window>();
        cppgui::sdl::Application<Main_window> app;

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