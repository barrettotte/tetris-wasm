#ifndef TETRIS_H
#define TETRIS_H

#include <stdint.h>

// constants

static const char* WINDOW_TITLE = "Tetris";

static const int CELL_SIZE = 40;
static const int GRID_COLS = 10;
static const int GRID_ROWS = 20;
static const Color GRID_COLOR = GRAY;

static const int WINDOW_PAD_X = 20;
static const int WINDOW_PAD_Y = 20;
static const int WINDOW_WIDTH = CELL_SIZE * GRID_COLS + (2 * WINDOW_PAD_X);
static const int WINDOW_HEIGHT = CELL_SIZE * GRID_ROWS + (2 * WINDOW_PAD_Y);

// types

typedef enum GameState {
    GAME_STATE_PLAY,
    GAME_STATE_OVER,
    GAME_STATE_PAUSE,
} GameState;

typedef struct {
    GameState state;
    uint64_t score;
} Game;

#endif
