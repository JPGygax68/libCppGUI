#include <iostream>

#include <SDL_main.h>
#include "./Application.hpp"

int main(int argc, char *argv[])
{
    try
    {
        Application app;

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