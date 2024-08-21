#ifndef TETRIS_H
#define TETRIS_H

#include "raylib.h"
#include <stdint.h>

// constants

#define GRID_COLS 10
#define GRID_ROWS 20

// for properly drawing tetromino patterns
#define GRID_HIDDEN_ROWS 2

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
static const int WINDOW_WIDTH = (GRID_CELL_SIZE * GRID_COLS) + (2 * GRID_PAD_X);
static const int WINDOW_HEIGHT = (GRID_CELL_SIZE * (GRID_ROWS + GRID_HIDDEN_ROWS)) + GRID_PAD_Y;
static const Color WINDOW_BACKGROUND = GRAY;

static const Color TEXT_COLOR = WHITE;
static const int SCORE_FONT_SIZE = 30;
static const int HEADER_HEIGHT = GRID_PAD_Y * 3;

static const Color GRID_LINE_COLOR = GRAY;
static const Color GRID_CELL_COLOR = BLACK;

static const int SPAWN_X = 3;
static const int SPAWN_Y = 0;
static const float FALL_SPEED = 0.35f;

static const int PATTERN_I[4] = {0b0000111100000000, 0b0010001000100010, 0b0000000011110000, 0b0100010001000100};
static const int PATTERN_O[4] = {0b0000011001100000, 0b0000011001100000, 0b0000011001100000, 0b0000011001100000};
static const int PATTERN_T[4] = {0b0000111001000000, 0b0100110001000000, 0b0100111000000000, 0b0100011001000000};
static const int PATTERN_J[4] = {0b0100010011000000, 0b1000111000000000, 0b0110010001000000, 0b0000111000100000};
static const int PATTERN_L[4] = {0b0100010001100000, 0b0000111010000000, 0b1100010001000000, 0b0010111000000000};
static const int PATTERN_S[4] = {0b0000011011000000, 0b1000110001000000, 0b0000011011000000, 0b1000110001000000};
static const int PATTERN_Z[4] = {0b0000110001100000, 0b0100110010000000, 0b0000110001100000, 0b0100110010000000};

// types

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
    EVENT_FALL,
    EVENT_INSTANT_FALL
} Event;

typedef enum CellState {
    CELL_STATE_CYAN,
    CELL_STATE_YELLOW,
    CELL_STATE_PURPLE,
    CELL_STATE_BLUE,
    CELL_STATE_ORANGE,
    CELL_STATE_GREEN,
    CELL_STATE_RED,
    CELL_STATE_EMPTY,
} CellState;

typedef enum TetrominoType {
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_TYPE_SIZE
} TetrominoType;

typedef struct {
    Color color;
    int patterns[4];
    TetrominoType type;
} Tetromino;

typedef struct {
    Tetromino* t;
    int x;
    int y;
    int rotIdx;
} MovableTetromino;

typedef struct {
    double duration;
    double prevTime;
} Timer;

typedef struct {
    GameState state;
    MovableTetromino* tetromino;
    Timer* fallTimer;
    int score;
} Game;

#endif
