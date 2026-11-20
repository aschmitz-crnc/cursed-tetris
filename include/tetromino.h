#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdbool.h>

/*
 * Every piece lives in a PIECE_BOX x PIECE_BOX bounding box, SRS-style:
 * the I and O pieces use the full 4x4 box, the 3x3 pieces (T, S, Z, J, L)
 * occupy its top-left 3x3 region.  This keeps collision, rotation and
 * rendering code uniform for all seven pieces.
 */
#define PIECE_BOX 4

/* Tetromino types */
typedef enum {
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_COUNT
} tetromino_type;

/* Rotation states (0 = spawn orientation) */
typedef enum {
    ROTATION_0,
    ROTATION_1,
    ROTATION_2,
    ROTATION_3,
    ROTATION_COUNT
} rotation;

/* A falling piece: (x, y) is the board position of the top-left corner
 * of its bounding box. */
typedef struct {
    tetromino_type type;
    rotation rotation;
    int x;
    int y;
} piece_t;

/* Spawn position: centered horizontally at the top of the board. */
#define PIECE_SPAWN_X 3
#define PIECE_SPAWN_Y 0

/* SRS shape tables: SHAPES[type][rotation][row][col], 1 = filled cell.
 * Defined in tetromino.c. */
extern const int SHAPES[TETROMINO_COUNT][ROTATION_COUNT][PIECE_BOX][PIECE_BOX];

/* Shape matrix for a piece type in a given rotation: a pointer to the
 * first row of the 4x4 matrix, so shape[r][c] is a cell. */
static inline const int (*shape_of(tetromino_type type, rotation rot))[PIECE_BOX]
{
    return SHAPES[type][rot];
}

/* Piece colors, mapped to ncurses base colors. */
typedef enum {
    COLOR_PIECE_I = 6,  /* cyan    */
    COLOR_PIECE_O = 3,  /* yellow  */
    COLOR_PIECE_T = 5,  /* magenta */
    COLOR_PIECE_S = 2,  /* green   */
    COLOR_PIECE_Z = 1,  /* red     */
    COLOR_PIECE_J = 4,  /* blue    */
    COLOR_PIECE_L = 7   /* white   */
} piece_color;

static inline piece_color get_color(tetromino_type type)
{
    switch (type) {
    case TETROMINO_I: return COLOR_PIECE_I;
    case TETROMINO_O: return COLOR_PIECE_O;
    case TETROMINO_T: return COLOR_PIECE_T;
    case TETROMINO_S: return COLOR_PIECE_S;
    case TETROMINO_Z: return COLOR_PIECE_Z;
    case TETROMINO_J: return COLOR_PIECE_J;
    case TETROMINO_L: return COLOR_PIECE_L;
    default: return COLOR_PIECE_I;
    }
}

/* Display character for a piece type. */
static inline char get_symbol(tetromino_type type)
{
    switch (type) {
    case TETROMINO_I: return 'I';
    case TETROMINO_O: return 'O';
    case TETROMINO_T: return 'T';
    case TETROMINO_S: return 'S';
    case TETROMINO_Z: return 'Z';
    case TETROMINO_J: return 'J';
    case TETROMINO_L: return 'L';
    default: return 'I';
    }
}

static inline const char* get_name(tetromino_type type)
{
    switch (type) {
    case TETROMINO_I: return "I";
    case TETROMINO_O: return "O";
    case TETROMINO_T: return "T";
    case TETROMINO_S: return "S";
    case TETROMINO_Z: return "Z";
    case TETROMINO_J: return "J";
    case TETROMINO_L: return "L";
    default: return "I";
    }
}

/* Create a piece of the given type at (x, y) with the given rotation. */
piece_t create_tetromino(tetromino_type type, rotation rot, int x, int y);

#endif /* TETROMINO_H */
