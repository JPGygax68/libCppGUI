#include <iostream>

#include <SDL_main.h>
#include "./Application.hpp"

int main(int argc, char *argv[])
{
    Application app;

    app.run();

    std::cout << "Press RETURN to terminate" << std::endl;
    char ch; std::cin >> std::noskipws >> ch;

    return 0;
}