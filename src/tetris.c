#include "tetris.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

static Game* game = NULL;
static CellState grid[GRID_ROWS + GRID_HIDDEN_ROWS][GRID_COLS];

// generate tetromino properties given tetromino type
static void generateTetromino(MovableTetromino* tetromino) {
    tetromino->x = SPAWN_X;
    tetromino->y = SPAWN_Y;
    tetromino->rotIdx = 0;

    TetrominoType type = (TetrominoType) (rand() % TETROMINO_TYPE_SIZE);
    tetromino->t->type = type;

    switch (type) {
        case TETROMINO_I:
            tetromino->t->color = TETRIS_CYAN;
            memcpy(tetromino->t->patterns, PATTERN_I, sizeof(int) * 4);
            break;
        case TETROMINO_O:
            tetromino->t->color = TETRIS_YELLOW;
            memcpy(tetromino->t->patterns, PATTERN_O, sizeof(int) * 4);
            break;
        case TETROMINO_T:
            tetromino->t->color = TETRIS_PURPLE;
            memcpy(tetromino->t->patterns, PATTERN_T, sizeof(int) * 4);
            break;
        case TETROMINO_J:
            tetromino->t->color = TETRIS_BLUE;
            memcpy(tetromino->t->patterns, PATTERN_J, sizeof(int) * 4);
            break;
        case TETROMINO_L:
            tetromino->t->color = TETRIS_ORANGE;
            memcpy(tetromino->t->patterns, PATTERN_L, sizeof(int) * 4);
            break;
        case TETROMINO_S:
            tetromino->t->color = TETRIS_GREEN;
            memcpy(tetromino->t->patterns, PATTERN_S, sizeof(int) * 4);
            break;
        case TETROMINO_Z:
            tetromino->t->color = TETRIS_RED;
            memcpy(tetromino->t->patterns, PATTERN_Z, sizeof(int) * 4);
            break;
        default:
            printf("Invalid tetromino type %d\n", type);
            exit(1);
            break;
    }
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

// set initial game state
static void init() {
    srand(time(NULL));

    MovableTetromino* current = malloc(sizeof(MovableTetromino));
    current->t = malloc(sizeof(Tetromino));
    generateTetromino(current);

    // init game data
    game = malloc(sizeof(Game));
    game->state = GAME_STATE_PLAY;
    game->score = 0;
    game->tetromino = current;

    // init timer for falling tetrominos
    game->fallTimer = malloc(sizeof(Timer));
    game->fallTimer->duration = FALL_SPEED;
    game->fallTimer->prevTime = GetTime();

    // init grid cells
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < (GRID_ROWS + GRID_HIDDEN_ROWS); i++) {
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
    free(game->fallTimer);
    free(game);
}

// check if timer has elapsed for timer duration
static bool timerHasFired(Timer* timer) {
    double now = GetTime();

    if (now - timer->prevTime >= timer->duration) {
        timer->prevTime = now;
        return true;
    }
    return false;
}

// check for events to process
static Event checkForEvent() {
    if (timerHasFired(game->fallTimer)) {
        return EVENT_FALL;
    }

    // input related events
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        return EVENT_ROTATE;
    } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        return EVENT_LEFT;
    } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        return EVENT_RIGHT;
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        return EVENT_DOWN;
    } else if (IsKeyPressed(KEY_SPACE)) {
        return EVENT_INSTANT_FALL;
    }
    return EVENT_NONE;
}

// check if tetromino pattern is within grid bounds
static bool isPatternInBounds(Vector2 pattern[4]) {
    for (int i = 0; i < 4; i++) {
        int x = pattern[i].x;
        int y = pattern[i].y;
        bool xOutOfBounds = (x < 0) || (x >= GRID_COLS);
        bool yOutOutOfBounds = (y < GRID_HIDDEN_ROWS) || (y >= (GRID_ROWS + GRID_HIDDEN_ROWS));

        if (grid[y][x] != CELL_STATE_EMPTY || xOutOfBounds || yOutOutOfBounds) {
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
                pattern[i].y = tetromino->y + y;
                i++;
            }
            row >>= 1; // advance to next bit of pattern row
        }
    }
}

// lock final position of current moving tetromino
static void lockTetramino(MovableTetromino* tetromino) {
    Vector2 pattern[4];
    getTetrominoPattern(tetromino, pattern);

    if (!isPatternInBounds(pattern)) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        int x = pattern[i].x;
        int y = pattern[i].y;
        grid[y][x] = (int) tetromino->t->type;
    }

    // set next tetromino
    generateTetromino(tetromino);
}

// update grid with updated moving tetromino
static void updateGrid(Vector2* currentPattern, MovableTetromino* updatedTetromino, Event event) {
    // clear moving tetramino
    for (int i = 0; i < 4; i++) {
        int x = currentPattern[i].x;
        int y = currentPattern[i].y;
        grid[y][x] = CELL_STATE_EMPTY;
    }

    Vector2 updatedPattern[4];
    getTetrominoPattern(updatedTetromino, updatedPattern);
    bool inBounds = isPatternInBounds(updatedPattern);

    if (inBounds) {
        game->tetromino->x = updatedTetromino->x;
        game->tetromino->y = updatedTetromino->y;
        game->tetromino->rotIdx = updatedTetromino->rotIdx;
    }

    // lock moving tetromino if hit bounds
    if ((!inBounds && event == EVENT_FALL) || (event == EVENT_INSTANT_FALL)) {
        lockTetramino(game->tetromino);
    }

    // TODO: check clear blocks?
}

// handle event
static void handleEvent(Event event) {
    Vector2 currentPattern[4] = {0};
    getTetrominoPattern(game->tetromino, currentPattern);

    MovableTetromino updated = *game->tetromino;

    if (event == EVENT_FALL) {
        updated.y++;
    }

    // check for input related events
    if (event == EVENT_ROTATE) {
        updated.rotIdx = (updated.rotIdx + 1) % 4;
    } else if (event == EVENT_LEFT) {
        updated.x--;
    } else if (event == EVENT_RIGHT) {
        updated.x++;
    } else if (event == EVENT_DOWN) {
        updated.y++;
    } else if (event == EVENT_INSTANT_FALL) {
        // TODO:
    }

    updateGrid(currentPattern, &updated, event);
}

// process game logic for single frame
static void update() {
    Event event = checkForEvent();

    if (event != EVENT_NONE) {
        handleEvent(event);
    }
}

// draw current score
static void drawScore(int score) {
    char buffer[32];
    sprintf(buffer, "Score: %d", score);

    int x = (WINDOW_WIDTH - MeasureText(buffer, SCORE_FONT_SIZE)) / 2;
    int y = HEADER_HEIGHT / 2;
    DrawText(buffer, x, y, SCORE_FONT_SIZE, TEXT_COLOR);
}

// draw a grid cell at given col,row
static void drawGridCell(int col, int row, Color color) {
    int x = (col * GRID_CELL_SIZE) + GRID_PAD_X;
    int y = (((row + 1) - GRID_HIDDEN_ROWS) * GRID_CELL_SIZE) + HEADER_HEIGHT - GRID_PAD_Y;

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
        int x = (i * GRID_CELL_SIZE) + GRID_PAD_X;
        DrawLine(x, GRID_PAD_Y, x, WINDOW_HEIGHT - GRID_PAD_Y, GRID_LINE_COLOR);
    }

    // horizontal lines
    for (int i = GRID_HIDDEN_ROWS; i <= (GRID_ROWS + GRID_HIDDEN_ROWS); i++) {
        int y = WINDOW_HEIGHT - ((i - GRID_HIDDEN_ROWS) * GRID_CELL_SIZE) - GRID_PAD_Y;
        DrawLine(GRID_PAD_X, y, WINDOW_WIDTH - GRID_PAD_X, y, GRID_LINE_COLOR);
    }

    // grid cells
    for (int i = GRID_HIDDEN_ROWS; i < (GRID_ROWS + GRID_HIDDEN_ROWS); i++) {
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
