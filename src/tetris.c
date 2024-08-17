#include "raylib.h"
#include "tetris.h"

#include <stdio.h>

// constants
static const int screenWidth = 800;
static const int screenHeight = 450;

// prototypes

static void init();
static void update();
static void draw();


// entry
int main(int argc, const char* argv[]) {
    printf("Tetris started.\n");
    InitWindow(screenWidth, screenHeight, "Tetris");
    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();
    printf("Tetris ended.\n");

    return 0;
}

// set initial game state
static void init() {
    SetTargetFPS(60);
}

// process updates during game loop
static void update() {
    
}

// draw updates during game loop
static void draw() {

}
