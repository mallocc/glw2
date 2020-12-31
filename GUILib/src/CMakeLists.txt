include_directories(${glwdev_SOURCE_DIR}/GUILib/inc)

file(GLOB GUILib_SRC "*.cpp")

add_library(GUILib ${GUILib_SRC})

# link libraries to the project to compile
target_link_libraries(GUILib)
