# tetris-c

Tetris clone in C and compiled to WASM.

This was made to learn the basics of Raylib, WASM, CMake, and Conan.

## Development

```sh
# init build system
conan install . --output-folder=build/ --build=missing
cmake -S . -B build tetris

# build
make -C build tetris

# run
./bin/tetris
```

### VS Code

For intellisense, add `compileCommands` property:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            // ...
            "compileCommands": "${workspaceFolder}/build/compile_commands.json"
        }
    ],
    "version": 4
}
```

## References

- https://tetris.wiki/Tetris.wiki
- https://emscripten.org/
