# tetris-c

Tetris clone in C and compiled to WASM.

This was made to learn the basics of Raylib, WASM, CMake, Conan, and Ninja.

TODO: screenshot

Served at TODO: github page url

## Controls

<ul>
  <li>Left: <kbd>&#8592;</kbd>, <kbd>A</kbd></li>
  <li>Right: <kbd>&#8594;</kbd>, <kbd>D</kbd></li>
  <li>Rotate: <kbd>&#8593;</kbd>, <kbd>W</kbd></li>
  <li>Down: <kbd>&#8595;</kbd>, <kbd>S</kbd></li>
  <li>Fast drop: <kbd>SPACE</kbd></li>
  <li>Restart: <kbd>R</kbd></li>
</ul>

## Development

Dependencies: CMake, Conan, Ninja, Emscripten

```sh
# native build
conan install . --profile=profiles/default --output-folder=build --build=missing
cmake -G Ninja -B build
ninja -C build

# build and run binary
ninja -C build run
```

```sh
# wasm build
conan install . --profile=profiles/wasm --output-folder=build-wasm --build=missing
cmake -G Ninja -B build-wasm -DEMSCRIPTEN=ON
ninja -C build-wasm

# build and serve wasm binary at http://localhost:1337
ninja -C build-wasm serve
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

## References

- https://tetris.wiki/Tetris.wiki
  - https://tetris.wiki/Scoring
- https://emscripten.org/
- https://en.wikipedia.org/wiki/Tetromino
- https://emscripten.org/docs/getting_started/downloads.html
- https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)
