include_directories(${glwdev_SOURCE_DIR}/Nain/inc)
include_directories(${glwdev_SOURCE_DIR}/GLWLib/inc)
link_directories(${glwdev_SOURCE_DIR}/GLWLib/src)

file(GLOB Main_SRC "*.cpp")

add_executable(Main ${Main_SRC})
target_link_libraries(Main GLWLib)
