#include "raylib.h"
#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>

// prototypes

static void init();
static void update();
static void draw();

// globals

static Game* game = NULL;


// entry
int main(int argc, const char* argv[]) {
    printf("%s started.\n", WINDOW_TITLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();
    printf("%s ended.\n", WINDOW_TITLE);

    return 0;
}

// set initial game state
static void init() {
    game = malloc(sizeof(Game));
    game->state = GAME_STATE_PLAY;
    game->score = 0;
}

// process game logic for one frame
static void update() {
    if (game->state == GAME_STATE_PLAY) {
        // TODO:
    }
}

static void drawScore(uint64_t score) {
    char t[32];
    sprintf(t, "Score: %lu", score);

    int x = (WINDOW_WIDTH - MeasureText(t, 30)) / 2;
    int y = WINDOW_PAD_Y;
    DrawText(t, x, y, 30, BLACK);
}

static void drawGrid() {
    for (int i = 0; i <= GRID_ROWS; i++) {
        int y = WINDOW_HEIGHT - WINDOW_PAD_Y - i * CELL_SIZE;
        DrawLine(WINDOW_PAD_X, y, WINDOW_WIDTH - WINDOW_PAD_X, y, GRID_COLOR);
    }
    for (int i = 0; i <= GRID_COLS; i++) {
        int x = WINDOW_PAD_X + i * CELL_SIZE;
        DrawLine(x, WINDOW_PAD_Y, x, WINDOW_HEIGHT - WINDOW_PAD_Y, GRID_COLOR);
    }
}

// draw graphics for one frame
static void draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (game->state == GAME_STATE_PLAY) {
        drawScore(game->score);
        drawGrid();
    }

    EndDrawing();
}
