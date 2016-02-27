project (test_app)

add_executable( ${PROJECT_NAME}
  
  main.cpp

  Window.cpp Window.hpp
)

target_link_libraries( ${PROJECT_NAME} PRIVATE SDL2-static SDL2main libCppGUI)
target_include_directories( ${PROJECT_NAME} PRIVATE SDL2-static)