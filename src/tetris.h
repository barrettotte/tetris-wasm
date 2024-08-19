#ifndef TETRIS_H
#define TETRIS_H

#include "raylib.h"
#include <stdint.h>

// constants

#define GRID_COLS 10
#define GRID_ROWS 20

#define TETRIS_CYAN   (Color) {0x00, 0xff, 0xff, 0xff}
#define TETRIS_YELLOW (Color) {0xff, 0xff, 0x00, 0xff}
#define TETRIS_PURPLE (Color) {0x94, 0x00, 0xd3, 0xff}
#define TETRIS_GREEN  (Color) {0x00, 0xff, 0x00, 0xff}
#define TETRIS_RED    (Color) {0xff, 0x00, 0x00, 0xff}
#define TETRIS_BLUE   (Color) {0x00, 0x00, 0xff, 0xff}
#define TETRIS_ORANGE (Color) {0xff, 0xa5, 0x00, 0xff}

static const int GRID_CELL_SIZE = 40;
static const float GRID_CELL_GAP = 0.5f;
static const int GRID_PAD_X = 20;
static const int GRID_PAD_Y = 20;

static const char* WINDOW_TITLE = "Tetris";
static const int WINDOW_WIDTH = GRID_CELL_SIZE * GRID_COLS + (2 * GRID_PAD_X);
static const int WINDOW_HEIGHT = GRID_CELL_SIZE * GRID_ROWS + (2 * GRID_PAD_Y);
static const Color WINDOW_BACKGROUND = GRAY;

static const int SCORE_FONT_SIZE = 30;
static const Color SCORE_COLOR = WHITE;

static const Color GRID_LINE_COLOR = GRAY;
static const Color GRID_CELL_COLOR = BLACK;

static const float DROP_SPEED = 0.2f;

// types

typedef enum CellState {
    CELL_STATE_CYAN,
    CELL_STATE_YELLOW,
    CELL_STATE_PURPLE,
    CELL_STATE_GREEN,
    CELL_STATE_RED,
    CELL_STATE_BLUE,
    CELL_STATE_ORANGE,
    CELL_STATE_EMPTY,
} CellState;

typedef enum GameState {
    GAME_STATE_PLAY,
    GAME_STATE_OVER,
    GAME_STATE_PAUSE,
} GameState;

typedef enum Event {
    EVENT_NONE,
    EVENT_ROTATE,
    EVENT_LEFT,
    EVENT_RIGHT,
    EVENT_DOWN,
} Event;

// typedef enum TetrominoType {
//     TETROMINO_I,
//     TETROMINO_O,
//     TETROMINO_T,
//     TETROMINO_J,
//     TETROMINO_L,
//     TETROMINO_S,
//     TETROMINO_Z
// } TetrominoType;

typedef struct {
    Color color;
    int patterns[4];
} Tetromino;

typedef struct {
    Tetromino* t;
    int x;
    int y;
    int rotIdx;
} MovableTetromino;

typedef struct {
    double length;
    double prevTick;
} Timer;

typedef struct {
    GameState state;
    uint64_t score;
    MovableTetromino* tetromino;
} Game;

// Tetraminos

static const Tetromino TETROMINO_I = {
    .color = TETRIS_CYAN,
    .patterns = {0b0000111100000000, 0b0010001000100010, 0b0000000011110000, 0b0100010001000100}
};

static const Tetromino TETROMINO_O = {
    .color = TETRIS_YELLOW,
    .patterns = {0b0000011001100000, 0b0000011001100000, 0b0000011001100000, 0b0000011001100000}
};

static const Tetromino TETROMINO_T = {
    .color = TETRIS_PURPLE,
    .patterns = {0b0000111001000000, 0b0100110001000000, 0b0100111000000000, 0b0100011001000000}
};

static const Tetromino TETROMINO_J = {
    .color = TETRIS_BLUE,
    .patterns = {0b0100010011000000, 0b1000111000000000, 0b0110010001000000, 0b0000111000100000}
};

static const Tetromino TETROMINO_L = {
    .color = TETRIS_ORANGE,
    .patterns = {0b0100010001100000, 0b0000111010000000, 0b1100010001000000, 0b0010111000000000}
};

static const Tetromino TETROMINO_S = {
    .color = TETRIS_GREEN,
    .patterns = {0b0000011011000000, 0b1000110001000000, 0b0000011011000000, 0b1000110001000000}
};

static const Tetromino TETROMINO_Z = {
    .color = TETRIS_RED,
    .patterns = {0b0000110001100000, 0b0100110010000000, 0b0000110001100000, 0b0100110010000000}
};

#endif
