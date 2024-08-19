# tetris-c

Tetris clone in C and compiled to WASM.

This was made to learn the basics of Raylib, WASM, CMake, and Conan.

## Unimplemented Features

- Tetromino hold
- Tetromino preview
- Levels

## Development

```sh
# init build system
conan install . --output-folder=build/ --build=missing
cmake -S . -B build

# build
make -C build tetris

# run
./bin/tetris
```

### VS Code

For intellisense, add `compileCommands` property:

```js
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
- https://en.wikipedia.org/wiki/Tetromino
