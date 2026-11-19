#include "board.h"
#include <stdlib.h>
#include <string.h>

void board_init(board_t* board) {
    board_reset(board);
    board->lines_cleared = 0;
    board->score = 0;
    board->level = 1;
}

void board_reset(board_t* board) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            board->grid[x][y] = CELL_EMPTY;
        }
    }
}

bool board_is_empty(const board_t* board, int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return false;
    }
    return board->grid[x][y] == CELL_EMPTY;
}

bool board_place(const board_t* board, int x, int y, const char* symbol, int color) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board->grid[j][i] == CELL_EMPTY) {
                if (x <= j && j <= x + 3 && y <= i && i <= y + 2) {
                    board->grid[j][i] = color;
                    board->grid[j][i] += symbol[0] & 0xFF;  // Store symbol in lower bits
                    return true;
                }
            }
        }
    }
    return false;
}

bool board_can_place(const board_t* board, int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board->grid[x + j][y + i] != CELL_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool board_collision(const board_t* board, int x, int y, int width, int height) {
    // Check if piece is within bounds
    if (x < 0 || x + width >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return true;
    }

    // Check for collisions with existing blocks
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board->grid[x + j][y + i] != CELL_EMPTY) {
                return true;
            }
        }
    }
    return false;
}

bool board_can_move_left(const board_t* board, int x, int width) {
    if (x <= 0) {
        return true;
    }
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        if (board->grid[x - 1][i] != CELL_EMPTY) {
            return false;
        }
    }
    return true;
}

bool board_can_move_right(const board_t* board, int x, int width) {
    if (x + width >= BOARD_WIDTH) {
        return true;
    }
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        if (board->grid[x + width][i] != CELL_EMPTY) {
            return false;
        }
    }
    return true;
}

bool board_move_down(board_t* board, int x, int y) {
    if (y + 1 >= BOARD_HEIGHT) {
        return false;
    }
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        if (board->grid[x][i + 1] != CELL_EMPTY) {
            return false;
        }
    }
    board->grid[x][y] = board->grid[x][y + 1];
    board->grid[x][y + 1] = CELL_EMPTY;
    return true;
}

void board_clear_lines(board_t* board) {
    int lines_to_clear = 0;
    int y = BOARD_HEIGHT - 1;

    // Count lines from bottom up
    while (y >= 0) {
        bool line_full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->grid[x][y] == CELL_EMPTY) {
                line_full = false;
                break;
            }
        }

        if (line_full) {
            lines_to_clear++;
            y--;
        } else {
            y--;
        }
    }

    if (lines_to_clear > 0) {
        // Shift down
        for (int y = BOARD_HEIGHT - lines_to_clear - 1; y >= 0; y--) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board->grid[x][y + lines_to_clear] = board->grid[x][y];
                board->grid[x][y] = CELL_EMPTY;
            }
        }

        // Update stats
        board->lines_cleared += lines_to_clear;
        board->score += lines_to_clear * 100 * board->level;
        board->level = board->lines_cleared / 10 + 1;
    }
}

int board_get_next_type(const board_t* board) {
    return board->lines_cleared % TETROMINO_COUNT;
}

bool board_is_game_over(const board_t* board) {
    return board->grid[0][0] != CELL_EMPTY;
}

int board_get_lines(const board_t* board) {
    return board->lines_cleared;
}

int board_get_score(const board_t* board) {
    return board->score;
}

int board_get_level(const board_t* board) {
    return board->level;
}
