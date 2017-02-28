project (test_app)

# The main target

add_executable( ${PROJECT_NAME}
  
  main.cpp
  Test_window.hpp           Test_window.cpp

  SDL_window.hpp            SDL_window.cpp
  SDL_application.hpp       SDL_application.cpp
  SDL_exception.hpp

  # libcppgui_inst.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/headers)

target_link_libraries( ${PROJECT_NAME} 
PRIVATE 
    SDL2-static SDL2main glbinding 
    libCppGUI
    #libGPCGLWrappers libGPCGUIRenderer libGPCGUIGLRenderer libCppGUI libCppGUI-SDL
)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)