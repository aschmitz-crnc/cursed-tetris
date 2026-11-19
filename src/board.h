#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

// Board dimensions
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Cell states
typedef enum {
    CELL_EMPTY = 0,
    CELL_FILLED = 1
} cell_state;

// Board structure
typedef struct {
    int grid[BOARD_WIDTH][BOARD_HEIGHT];
    int lines_cleared;
    int score;
    int level;
} board_t;

// Initialize/reset board
void board_init(board_t* board);

// Reset board (clears all cells)
void board_reset(board_t* board);

// Check if cell is empty
bool board_is_empty(const board_t* board, int x, int y);

// Place tetromino on board
// Returns true if successful, false if collision
bool board_place(const board_t* board, int x, int y, const char* symbol, int color);

// Check if tetromino can be placed at position (without actually placing)
bool board_can_place(const board_t* board, int x, int y, int width, int height);

// Check for collisions with tetromino
bool board_collision(const board_t* board, int x, int y, int width, int height);

// Check if piece can move left
bool board_can_move_left(const board_t* board, int x, int width);

// Check if piece can move right
bool board_can_move_right(const board_t* board, int x, int width);

// Move piece down by one
bool board_move_down(board_t* board, int x, int y);

// Clear completed lines and update score
void board_clear_lines(board_t* board);

// Get next tetromino type
int board_get_next_type(const board_t* board);

// Check if game is over (new piece cannot spawn)
bool board_is_game_over(const board_t* board);

// Get board statistics
int board_get_lines(const board_t* board);
int board_get_score(const board_t* board);
int board_get_level(const board_t* board);

#endif // BOARD_H
