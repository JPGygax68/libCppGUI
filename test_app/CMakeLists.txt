project (test_app)

# The main target

add_executable( ${PROJECT_NAME}
  
  main.cpp

  Test_window.hpp           Test_window.cpp

  # libcppgui_inst.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/headers)

target_link_libraries( ${PROJECT_NAME} 
PRIVATE 
    SDL2-static SDL2main glbinding 
    #libGPCGLWrappers libGPCGUIRenderer libGPCGUIGLRenderer libCppGUI libCppGUI-SDL
)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)