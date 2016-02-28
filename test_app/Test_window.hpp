#pragma once

#include "./Window.hpp"

class Test_window : public Window<Test_window> {
public:
    Test_window();

    void init();
};