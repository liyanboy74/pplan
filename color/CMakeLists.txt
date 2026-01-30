cmake_minimum_required(VERSION 3.10)

project(color)

add_library(${PROJECT_NAME} color.c)

target_link_libraries(${PROJECT_NAME} m)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES color.h DESTINATION include)
