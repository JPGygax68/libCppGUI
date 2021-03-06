#include <iostream>
#include <memory>

#include <SDL_main.h>

#include <cppgui/Application.hpp>

#include "./Test_window.hpp"


int main(int /*argc*/, char * /*argv*/[])
{
    try
    {
        auto win = std::make_unique<Test_window>();
        cppgui::Application app;

        win->init();

        app.run();

        win->cleanup();

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