#include "tetris.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static Game* game = NULL;
static CellState grid[GRID_ROWS][GRID_COLS];

// set initial game state
static void init() {
    MovableTetromino* current = malloc(sizeof(MovableTetromino));
    current->rotIdx = 0;
    current->x = 3;
    current->y = 2;
    current->t = malloc(sizeof(Tetromino));
    memcpy(current->t, &TETROMINO_I, sizeof(Tetromino));
    
    // init game data
    game = malloc(sizeof(Game));
    game->state = GAME_STATE_PLAY;
    game->score = 0;
    game->tetromino = current;

    // init grid cells
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j] = CELL_STATE_EMPTY;
        }
    }

    // init UI
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
}

// tear down game
static void cleanup() {
    CloseWindow();
    free(game->tetromino->t);
    free(game->tetromino);
    free(game);
}

// check for events to process
static Event checkForEvent() {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        return EVENT_ROTATE;
    } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        return EVENT_LEFT;
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        return EVENT_RIGHT;
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        return EVENT_DOWN;
    }
    return EVENT_NONE;
}

// check if tetromino pattern is within grid bounds
static bool isPatternInBounds(Vector2 pattern[4]) {
    for (int i = 0; i < 4; i++) {
        uint8_t x = pattern[i].x;
        uint8_t y = pattern[i].y;

        if (grid[y][x] != CELL_STATE_EMPTY || (x < 0) || (x >= GRID_COLS) || (y < 0) || (y >= GRID_ROWS)) {
            return false;
        }
    }
    return true;
}

// get appropriate pattern of cells for tetramino based on its current rotation
static void getTetrominoPattern(MovableTetromino* tetromino, Vector2* pattern) {
    int i = 0;
    int p = tetromino->t->patterns[tetromino->rotIdx];

    // extract tetromino's block pattern from binary of pattern
    for (int y = 0; y < 4; y++) {
        int row = p >> (y * 4); // 4-bit row

        for (int x = 0; x < 4; x++) {
            // check if current bit of row is set
            if (row & 0x1) {
                pattern[i].x = tetromino->x + x;
                pattern[i].y = tetromino->y - y;
                i++;
            }
            row >>= 1; // advance to next bit of pattern row
        }
    }
}

// update grid with updated moving tetromino
static void updateGrid(Vector2* currentPattern, MovableTetromino* updatedTetromino) {
    // clear moving tetramino
    for (int i = 0; i < 4; i++) {
        int x = currentPattern[i].x;
        int y = currentPattern[i].y;
        grid[y][x] = CELL_STATE_EMPTY;
    }

    // update grid with moved tetramino if in bounds
    Vector2 updatedPattern[4];
    getTetrominoPattern(updatedTetromino, updatedPattern);

    if (isPatternInBounds(updatedPattern)) {
        game->tetromino->x = updatedTetromino->x;
        game->tetromino->y = updatedTetromino->y;
        game->tetromino->rotIdx = updatedTetromino->rotIdx;
    }
}

// handle event
static void handleEvent(Event event, MovableTetromino* tetromino) {
    if (event == EVENT_ROTATE) {
        tetromino->rotIdx = (tetromino->rotIdx + 1) % 4;
    } else if (event == EVENT_LEFT) {
        tetromino->x--;
    } else if (event == EVENT_RIGHT) {
        tetromino->x++;
    } else if (event == EVENT_DOWN) {
        tetromino->y++;
    }
}

// process game logic for single frame
static void update() {
    Event event = checkForEvent();

    if (event != EVENT_NONE) {
        Vector2 currentPattern[4];
        getTetrominoPattern(game->tetromino, currentPattern);

        MovableTetromino updatedTetromino = *game->tetromino;

        handleEvent(event, &updatedTetromino);
        updateGrid(currentPattern, &updatedTetromino);
    }
}

// draw current score
static void drawScore(uint64_t score) {
    char buffer[32];
    sprintf(buffer, "Score: %lu", score);

    int x = (WINDOW_WIDTH - MeasureText(buffer, SCORE_FONT_SIZE)) / 2;
    int y = GRID_PAD_Y;
    DrawText(buffer, x, y, SCORE_FONT_SIZE, SCORE_COLOR);
}

// get color for the given cell state
static Color cellStateToColor(CellState state) {
    switch (state) {
        case CELL_STATE_CYAN:   return TETRIS_CYAN;
        case CELL_STATE_YELLOW: return TETRIS_YELLOW;
        case CELL_STATE_PURPLE: return TETRIS_PURPLE;
        case CELL_STATE_GREEN:  return TETRIS_GREEN;
        case CELL_STATE_RED:    return TETRIS_RED;
        case CELL_STATE_BLUE:   return TETRIS_BLUE;
        case CELL_STATE_ORANGE: return TETRIS_ORANGE;
        case CELL_STATE_EMPTY:  return GRID_CELL_COLOR;
        default:                return MAGENTA; // invalid
    }
}

// draw a grid cell at given col,row
static void drawGridCell(int col, int row, Color color) {
    int x = (col * GRID_CELL_SIZE) + GRID_PAD_X;
    int y = ((row + 1) * GRID_CELL_SIZE) - GRID_PAD_Y;

    Rectangle rect = {
        .x = x + GRID_CELL_GAP,
        .y = y + GRID_CELL_GAP,
        .width = GRID_CELL_SIZE - (2 * GRID_CELL_GAP),
        .height = GRID_CELL_SIZE - (2 * GRID_CELL_GAP)
    };
    DrawRectangleRec(rect, color);
}

// draw grid where tetrominos sit
static void drawGrid() {
    // vertical lines
    for (int i = 0; i <= GRID_COLS; i++) {
        int x = GRID_PAD_X + i * GRID_CELL_SIZE;
        DrawLine(x, GRID_PAD_Y, x, WINDOW_HEIGHT - GRID_PAD_Y, GRID_LINE_COLOR);
    }

    // horizontal lines
    for (int i = 0; i <= GRID_ROWS; i++) {
        int y = WINDOW_HEIGHT - GRID_PAD_Y - i * GRID_CELL_SIZE;
        DrawLine(GRID_PAD_X, y, WINDOW_WIDTH - GRID_PAD_X, y, GRID_LINE_COLOR);
    }

    // grid cells
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            drawGridCell(j, i, cellStateToColor(grid[i][j]));
        }
    }
}

// draw tetromino to grid
static void drawTetromino(MovableTetromino* tetromino) {
    Vector2 pattern[4];
    getTetrominoPattern(tetromino, pattern);

    // make sure tetromino can be drawn in bounds
    if (!isPatternInBounds(pattern)) {
        return;
    }

    // draw tetromino cell by cell
    for (int i = 0; i < 4; i++) {
        drawGridCell(pattern[i].x, pattern[i].y, tetromino->t->color);
    }
}

// draw graphics for single frame
static void draw() {
    BeginDrawing();
    ClearBackground(WINDOW_BACKGROUND);

    if (game->state == GAME_STATE_PLAY) {
        drawGrid();
        drawTetromino(game->tetromino);
        drawScore(game->score);
    }
    EndDrawing();
}

// program entry
int main(int argc, const char* argv[]) {
    printf("%s started.\n", WINDOW_TITLE);
    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    cleanup();
    printf("%s ended.\n", WINDOW_TITLE);
    return 0;
}
