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
    } else if (IsKeyPressed(KEY_R)) {
        return EVENT_RESTART;
    }
    return EVENT_NONE;
}

// reset game to initial state
static void restart() {
    game->score = 0;
    game->state = GAME_STATE_PLAY;
    game->fallTimer->duration = FALL_SPEED;
    game->fallTimer->prevTime = GetTime();

    // init grid cells
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < (GRID_ROWS + GRID_HIDDEN_ROWS); i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j] = CELL_STATE_EMPTY;
        }
    }

    // set initial tetromino
    generateTetromino(game->tetromino);
}

// check if tetromino's cells are within grid bounds and can continue moving
static bool isTetrominoMovable(Vector2 cells[4]) {
    for (int i = 0; i < 4; i++) {
        int x = cells[i].x;
        int y = cells[i].y;
        bool xOutOfBounds = (x < 0) || (x >= GRID_COLS);
        bool yOutOutOfBounds = (y < GRID_HIDDEN_ROWS) || (y >= (GRID_ROWS + GRID_HIDDEN_ROWS));

        if (grid[y][x] != CELL_STATE_EMPTY || xOutOfBounds || yOutOutOfBounds) {
            return false;
        }
    }
    return true;
}

// get appropriate cells for tetramino based on its current rotation
static void getTetrominoCells(MovableTetromino* tetromino, Vector2* cells) {
    int i = 0;
    int p = tetromino->t->patterns[tetromino->rotIdx];

    // extract tetromino's block pattern from binary of pattern
    for (int y = 0; y < 4; y++) {
        int row = p >> (y * 4); // 4-bit row

        for (int x = 0; x < 4; x++) {
            // check if current bit of row is set
            if (row & 0x1) {
                cells[i].x = tetromino->x + x;
                cells[i].y = tetromino->y + y;
                i++;
            }
            row >>= 1; // advance to next bit of pattern row
        }
    }
}

// clear completed rows, returning rows cleared
static int clearFullRows() {
    int rows = 0;

    for (int y = GRID_HIDDEN_ROWS; y < (GRID_ROWS + GRID_HIDDEN_ROWS); y++) {
        bool full = true;

        // check if row is full
        for (int x = 0; x < GRID_COLS; x++) {
            if (grid[y][x] == CELL_STATE_EMPTY) {
                full = false;
                break;
            }
        }

        // move rows above down to current row
        if (full) {
            for (int j = y; j > GRID_HIDDEN_ROWS; j--) {
                memcpy(grid[j], grid[j-1], sizeof(grid[0]));
            }
            y--; // recheck current row after clearing it
            rows++;
        }
    }
    return rows;
}

// lock final position of current moving tetromino
static void lockTetramino(MovableTetromino* tetromino) {
    Vector2 cells[4];
    getTetrominoCells(tetromino, cells);

    // update cells to lock final position
    for (int i = 0; i < 4; i++) {
        int x = cells[i].x;
        int y = cells[i].y;
        grid[y][x] = (int) tetromino->t->type;
    }
}

// update grid with updated moving tetromino
static void updateGrid(Vector2* cells, MovableTetromino* tetromino, Event event) {
    for (int i = 0; i < 4; i++) {
        int x = cells[i].x;
        int y = cells[i].y;

        if (y < GRID_HIDDEN_ROWS) {
            y = GRID_HIDDEN_ROWS;
        }
        grid[y][x] = CELL_STATE_EMPTY; // clear moving tetromino
    }

    Vector2 updatedCells[4];
    getTetrominoCells(tetromino, updatedCells);
    bool movable = isTetrominoMovable(updatedCells);

    if (movable) {
        game->tetromino->x = tetromino->x;
        game->tetromino->y = tetromino->y;
        game->tetromino->rotIdx = tetromino->rotIdx;
    }

    // lock moving tetromino if hit bounds
    if ((!movable && event == EVENT_FALL) || (tetromino->y >= GRID_HIDDEN_ROWS && event == EVENT_INSTANT_FALL)) {
        lockTetramino(game->tetromino);

        // clear completed rows
        int rows = clearFullRows();
        if (rows > 0) {
            rows = rows > 4 ? 4 : rows;
            game->score += ROWS_TO_POINTS[rows-1];
        }

        // set next tetromino
        generateTetromino(game->tetromino);
    }
}

// handle event
static void handleEvent(Event event) {
    if (game->state == GAME_STATE_OVER) {
        if (event == EVENT_RESTART) {
            restart();
        }
        return; // stay at game over screen
    }

    // restart to initial state
    if (event == EVENT_RESTART) {
        restart();
    }

    Vector2 cells[4] = {0};
    getTetrominoCells(game->tetromino, cells);

    MovableTetromino updated = *game->tetromino;

    // regular tetromino fall on interval
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
        while (isTetrominoMovable(cells)) {
            updated.y++;
            getTetrominoCells(&updated, cells);
        }
        updated.y--;
        getTetrominoCells(&updated, cells);
    }

    // update grid with changes to current tetromino
    updateGrid(cells, &updated, event);
}

// check if blocks have hit the top of the grid, triggering game over
static void checkForGameOver() {
    for (int x = 0; x < GRID_COLS; x++) {
        if (grid[GRID_HIDDEN_ROWS][x] != CELL_STATE_EMPTY) {
            game->state = GAME_STATE_OVER;
        }
    }
}

// draw title above grid
static void drawTitle() {
    char buffer[64];
    sprintf(buffer, "%s", WINDOW_TITLE);

    int x = (WINDOW_WIDTH - MeasureText(buffer, TITLE_FONT_SIZE)) / 2;
    int y = 2;
    DrawText(buffer, x, y, TITLE_FONT_SIZE, TEXT_COLOR);
}

// draw current score above grid
static void drawScore(unsigned long score) {
    char buffer[64];
    sprintf(buffer, "Score: %lu", score);

    int x = (WINDOW_WIDTH - MeasureText(buffer, SCORE_FONT_SIZE)) / 2;
    int y = (HEADER_HEIGHT / 2) + 10;
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
    Vector2 cells[4];
    getTetrominoCells(tetromino, cells);

    // leave if we dont need to redraw tetromino
    if (!isTetrominoMovable(cells)) {
        return;
    }

    // draw tetromino cell by cell
    for (int i = 0; i < 4; i++) {
        drawGridCell(cells[i].x, cells[i].y, tetromino->t->color);
    }
}

// draw game over message
static void drawGameOver() {
    int x = (WINDOW_WIDTH - MeasureText("Game Over", 48)) / 2;
    int y = WINDOW_HEIGHT / 2;

    DrawText("Game Over", x, y, 48, TEXT_COLOR);
    DrawText("Press R to Restart", x, y + 48, 24, TEXT_COLOR);
}

// set initial game state
static void init() {
    srand(time(NULL));

    // allocate memory
    game = malloc(sizeof(Game));
    game->tetromino = malloc(sizeof(MovableTetromino));
    game->tetromino->t = malloc(sizeof(Tetromino));
    game->fallTimer = malloc(sizeof(Timer));

    // reset state
    restart();
}

// draw graphics for single frame
static void draw() {
    BeginDrawing();
    ClearBackground(WINDOW_BACKGROUND);

    if (game->state == GAME_STATE_PLAY) {
        drawGrid();
        drawTetromino(game->tetromino);
        drawScore(game->score);
        drawTitle();
    } else if (game->state == GAME_STATE_OVER) {
        drawGameOver();
    }
    EndDrawing();
}

// process game logic for single frame
static void update() {
    Event event = checkForEvent();

    if (event != EVENT_NONE) {
        handleEvent(event);
    }
    checkForGameOver();
}

// tear down game
static void cleanup() {
    free(game->tetromino->t);
    free(game->tetromino);
    free(game->fallTimer);
    free(game);
}

// program entry
int main(int argc, const char* argv[]) {
    printf("%s started.\n", WINDOW_TITLE);
    init();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        update();
        draw();
    }
    CloseWindow();

    cleanup();
    printf("%s ended.\n", WINDOW_TITLE);
    return 0;
}
