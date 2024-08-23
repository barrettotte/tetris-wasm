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
# build system
conan install . -pr=./profiles/default --build=missing
cmake -G Ninja --preset conan-release

# build binary
ninja -C build/Release

# build and run binary
ninja -C build/Release run
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
