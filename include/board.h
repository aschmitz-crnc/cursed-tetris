#ifndef BOARD_H
#define BOARD_H

#include "tetromino.h"
#include <stdbool.h>

/* Board dimensions (standard playfield) */
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct {
    /* grid[x][y]: 0 = empty, otherwise (tetromino_type + 1) */
    int grid[BOARD_WIDTH][BOARD_HEIGHT];
    int lines_cleared;
    int score;
    int level;
} board_t;

/* Reset grid and statistics */
void board_init(board_t* board);

/* Clear all cells (keeps statistics) */
void board_reset(board_t* board);

/* Would the piece collide at its current position?
 * Cells above the top of the board (y < 0) count as empty. */
bool board_check(const board_t* board, const piece_t* piece);

/* Would the piece collide if offset by (dx, dy)? */
bool board_check_offset(const board_t* board, const piece_t* piece,
                        int dx, int dy);

/* Write the piece's cells into the grid (locking it in place). */
void board_lock(board_t* board, const piece_t* piece);

/* Remove all completed rows and shift the rest down.
 * Returns the number of rows cleared (0-4). */
int board_clear_lines(board_t* board);

int board_get_lines(const board_t* board);
int board_get_score(const board_t* board);
int board_get_level(const board_t* board);

#endif /* BOARD_H */
