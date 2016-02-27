#include <iostream>

#include <SDL_main.h>

int main(int argc, char *argv[])
{
    std::cout << "Press RETURN to terminate" << std::endl;
    char ch; std::cin >> std::noskipws >> ch;

    return 0;
}