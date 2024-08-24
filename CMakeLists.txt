cmake_minimum_required(VERSION 3.5)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(BUILD_DIR "build-wasm")
else()
    set(BUILD_DIR "build")
endif ()

# set toolchain
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/${BUILD_DIR}/build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake")

# for vscode intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set project name and language
project(tetris C)

# dependencies
find_package(raylib REQUIRED)

# project config
add_executable(${PROJECT_NAME} src/tetris.c)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
target_link_libraries(${PROJECT_NAME} raylib)

# add convenient run commands
if (EMSCRIPTEN)
    if(NOT WASM_PORT)
        set(WASM_PORT "1337")
    endif()

    # serve wasm build locally
    add_custom_target(serve
        COMMENT "Serving ${PROJECT_NAME} web build at http://localhost:${WASM_PORT}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/docs"    
        COMMAND cp "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}.js" .
        COMMAND cp "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}.wasm" .
        COMMAND python3 -m http.server "${WASM_PORT}"
    )
else()
    # run native build
    add_custom_target(run
        COMMENT "Running ${PROJECT_NAME}"
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMAND "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}"
    )
endif()
