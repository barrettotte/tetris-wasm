# tetris-c

Tetris clone in C and compiled to WASM.

This was made to learn the basics of Raylib, WASM, CMake, Conan, and Ninja.

## Unimplemented Features

- Tetromino ghost/shadow
- Next tetromino preview
- Tetromino hold
- Levels
- Pause screen
- Audio
- Limited scoring
- No high score recording
- Probably more I didn't notice

## Development

Dependencies: CMake, Conan, Ninja, Emscripten

```sh
# native build
conan install . --profile=profiles/default --output-folder=build --build=missing
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release
ninja -C build

# build and run binary
ninja -C build run
```

```sh
# wasm build
conan install . --profile=profiles/wasm --output-folder=build-wasm --build=missing
cmake -G Ninja -B build-wasm -DCMAKE_BUILD_TYPE=Release -DEMSCRIPTEN=ON
ninja -C build-wasm
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
  - https://tetris.wiki/Scoring
- https://emscripten.org/
- https://en.wikipedia.org/wiki/Tetromino
- https://emscripten.org/docs/getting_started/downloads.html
- https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)
