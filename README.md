# tetris-c

Tetris clone in C and compiled to WASM.

This was made to learn the basics of WASM, CMake, and Conan.

## Usage

TODO:

```sh
# init build system
conan install . --output-folder=build/ --build=missing
cmake -S . -B build tetris

# build
make -C build tetris

# run
./bin/tetris
```

## References

- https://tetris.wiki/Tetris.wiki
- https://emscripten.org/
