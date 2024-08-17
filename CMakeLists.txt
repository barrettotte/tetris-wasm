cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/build/conan_toolchain.cmake" CACHE STRING "Toolchain file")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build: Debug, Release, RelWithDebInfo, MinSizeRel" FORCE)
set(CMAKE_C_COMPILER "clang-18")

project(tetris C)

# dependencies
find_package(raylib REQUIRED)

# project
add_executable(${PROJECT_NAME} src/tetris.c)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
target_link_libraries(${PROJECT_NAME} raylib)
