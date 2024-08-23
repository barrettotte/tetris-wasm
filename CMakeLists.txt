cmake_minimum_required(VERSION 3.5)

# for vscode intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set project name and language
project(tetris C)

# dependencies
find_package(raylib REQUIRED)

# project config
add_executable(${PROJECT_NAME} src/tetris.c)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
target_link_libraries(${PROJECT_NAME} raylib)

# compile raylib to wasm
add_custom_target(raylib_wasm
    # COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/build/wasm/raylib
    COMMAND echo 'TODO: raylib_wasm'
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling raylib to WASM"
)

# compile project to wasm
add_custom_target(wasm
    DEPENDS ${PROJECT_NAME} raylib_wasm
    COMMAND echo 'TODO: wasm'
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling ${PROJECT_NAME} to WASM"
)

# run native build
add_custom_target(run
    DEPENDS ${PROJECT_NAME}
    COMMAND bin/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)

# serve wasm build locally
add_custom_target(serve
    COMMAND echo 'TODO: serve wasm build locally...'
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Serving ${PROJECT_NAME} web build at http://localhost:1337"
)
