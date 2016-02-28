project (test_app)

add_executable( ${PROJECT_NAME}
  
  main.cpp

  Test_window.hpp       Test_window.cpp
  Application.hpp       Application.ipp
  Window.hpp            Window.ipp 
)

target_link_libraries( ${PROJECT_NAME} 
PRIVATE 
    SDL2-static SDL2main GLBinding libGPCGLWrappers libGPCGUIRenderer libGPCGUIGLRenderer libCppGUI
)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)