from conan import ConanFile
from conan.tools.cmake import CMake,cmake_layout

class TetrisWasm(ConanFile):
    name = 'tetris-wasm'
    version = '1.0'
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeDeps', 'CMakeToolchain'
    requires = 'raylib/5.0'

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
