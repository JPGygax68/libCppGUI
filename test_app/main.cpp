#include <iostream>

#include <SDL_main.h>

#include "./Test_window.hpp"
#include "./Application.hpp"

// Explicitly instantiate Application and Window templates
#include "./Window.ipp"
#include "./Application.ipp"
Application<Test_window>;

int main(int /*argc*/, char * /*argv*/[])
{
    try
    {
        auto win = std::make_unique<Test_window>();
        Application<Test_window> app;

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