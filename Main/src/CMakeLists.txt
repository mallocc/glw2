include_directories(${glwdev_SOURCE_DIR}/Main/inc)
include_directories(${glwdev_SOURCE_DIR}/GLWLib/inc)
include_directories(${glwdev_SOURCE_DIR}/GUILib/inc)
link_directories(${glwdev_SOURCE_DIR}/GLWLib/src)
link_directories(${glwdev_SOURCE_DIR}/GUILib/src)

file(GLOB Main_SRC "*.cpp")

add_executable(Main ${Main_SRC})
target_link_libraries(Main GLWLib GUILib)
