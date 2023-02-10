project(gfx)

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

include(setup)
include(dependencies)

include_directories(3rdparty)
add_subdirectory(3rdparty/fmt EXCLUDE_FROM_ALL)

add_subdirectory(src)

add_subdirectory(test)

include(install)

