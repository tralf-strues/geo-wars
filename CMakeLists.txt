
# You are free to modify this file

cmake_minimum_required(VERSION 3.0)
project(GWARS)

find_package(X11 REQUIRED)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O3 -fsanitize=address -fno-optimize-sibling-calls")

file(GLOB SRC src/*.cpp)

add_executable(gwars ${SRC})

add_subdirectory(src)

target_include_directories(gwars PUBLIC ${X11_INCLUDE_DIR})
target_link_libraries(gwars m ${X11_LIBRARIES})