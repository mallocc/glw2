include_directories(${glwdev_SOURCE_DIR}/GLWLib/inc)

file(GLOB GLWLib_SRC "*.cpp")

add_library(GLWLib ${GLWLib_SRC})

# link libraries to the project to compile
target_link_libraries(GLWLib PUBLIC GL GLU GLEW glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)
