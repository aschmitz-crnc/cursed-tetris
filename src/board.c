#include "board.h"

void board_init(board_t* board)
{
    board_reset(board);
    board->lines_cleared = 0;
    board->score = 0;
    board->level = 1;
}

void board_reset(board_t* board)
{
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            board->grid[x][y] = 0;
        }
    }
}

bool board_check_offset(const board_t* board, const piece_t* piece,
                        int dx, int dy)
{
    const int (*shape)[PIECE_BOX] = shape_of(piece->type, piece->rotation);

    for (int r = 0; r < PIECE_BOX; r++) {
        for (int c = 0; c < PIECE_BOX; c++) {
            if (!shape[r][c])
                continue;

            int x = piece->x + dx + c;
            int y = piece->y + dy + r;

            /* Walls and floor */
            if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
                return true;
            /* Locked cells (above the top is allowed) */
            if (y >= 0 && board->grid[x][y] != 0)
                return true;
        }
    }
    return false;
}

bool board_check(const board_t* board, const piece_t* piece)
{
    return board_check_offset(board, piece, 0, 0);
}

void board_lock(board_t* board, const piece_t* piece)
{
    const int (*shape)[PIECE_BOX] = shape_of(piece->type, piece->rotation);
    int value = piece->type + 1;

    for (int r = 0; r < PIECE_BOX; r++) {
        for (int c = 0; c < PIECE_BOX; c++) {
            if (!shape[r][c])
                continue;

            int x = piece->x + c;
            int y = piece->y + r;
            if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
                board->grid[x][y] = value;
        }
    }
}

static bool row_is_full(const board_t* board, int y)
{
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (board->grid[x][y] == 0)
            return false;
    }
    return true;
}

int board_clear_lines(board_t* board)
{
    int cleared = 0;
    int write_y = BOARD_HEIGHT - 1;

    /* Walk bottom-up.  Surviving rows are compacted downward into the
     * slots left by full rows; every surviving row is cleared from its
     * old position, which also wipes any full row it lands on. */
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        if (row_is_full(board, y)) {
            cleared++;
            continue;
        }
        if (write_y != y) {
            for (int x = 0; x < BOARD_WIDTH; x++)
                board->grid[x][write_y] = board->grid[x][y];
            for (int x = 0; x < BOARD_WIDTH; x++)
                board->grid[x][y] = 0;
        }
        write_y--;
    }

    /* Safety: top rows must end up empty */
    for (; write_y >= 0; write_y--) {
        for (int x = 0; x < BOARD_WIDTH; x++)
            board->grid[x][write_y] = 0;
    }

    return cleared;
}

int board_get_lines(const board_t* board)
{
    return board->lines_cleared;
}

int board_get_score(const board_t* board)
{
    return board->score;
}

int board_get_level(const board_t* board)
{
    return board->level;
}
