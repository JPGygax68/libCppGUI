#include <iostream>

#include <SDL_main.h>

#include <cppgui/sdl/Application.hpp>

#include "./Test_window.hpp"

// Explicitly instantiate Application class template
#include <cppgui/sdl/Application.ipp>
#include <cppgui/sdl/Window.ipp>
template cppgui::sdl::Application<Test_window>;

int main(int /*argc*/, char * /*argv*/[])
{
    try
    {
        auto win = std::make_unique<Test_window>();
        cppgui::sdl::Application<Test_window> app;

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