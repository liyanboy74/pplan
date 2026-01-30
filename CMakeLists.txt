cmake_minimum_required(VERSION 3.10)

add_subdirectory(color)
add_subdirectory(simlcd)
add_subdirectory(dispcolor)

project(pplan)

add_executable(${PROJECT_NAME} main.cpp pplan.c)

include_directories(${SDL2_INCLUDE_DIRS} simlcd color dispcolor dispcolor/fonts)
target_link_libraries(${PROJECT_NAME} dispcolor simlcd color )
target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)

