#include <iostream>

#include <cppgui/geometry.hpp>

int main(int /*argc*/, char ** /*argv*/)
{
    using namespace cppgui;

    {
        std::cout << "Oriented_rectangle<>: can instantiate with all legal template parameter combinations" << std::endl;
        Oriented_rectangle<left_to_right, top_down> rect1;
    }

    return -1;
}