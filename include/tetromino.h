#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdbool.h>

// Tetromino types
typedef enum {
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_COUNT
} tetromino_t;

// Tetromino colors (NCurses colors)
typedef enum {
    COLOR_I = 6,      // Cyan
    COLOR_O = 3,      // Yellow
    COLOR_T = 5,      // Purple
    COLOR_S = 4,      // Green
    COLOR_Z = 1,      // Red
    COLOR_J = 2,      // Blue
    COLOR_L = 7       // Orange
} tetromino_color;

// Rotation states
typedef enum {
    ROTATION_0,
    ROTATION_1,
    ROTATION_2,
    ROTATION_3
} rotation;

// Shape matrices (1 = filled, 0 = empty)
// I piece - 4x2 (vertical)
static const int I_SHAPES[4][4][4] = {
    {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}},  // rotation 0 (horizontal)
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},  // rotation 1 (vertical)
    {{0,0,0,0}, {0,1,1,1}, {0,0,1,0}, {0,0,0,0}},  // rotation 2
    {{0,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}},  // rotation 3
};

// O piece - 2x2
static const int O_SHAPES[4][3][3] = {
    {{0,1,1}, {0,1,1}, {0,0,0}},  // rotation 0
    {{0,0,0}, {1,1}, {0,0}},      // rotation 1
    {{0,0,0}, {1,1}, {0,0}},      // rotation 2
    {{0,0,0}, {1,1}, {0,0}}       // rotation 3
};

// T piece - 3x3
static const int T_SHAPES[4][3][3] = {
    {{0,1,0}, {1,1,1}, {0,0,0}},  // rotation 0
    {{0,1,0}, {0,1,1}, {0,1,0}},  // rotation 1
    {{0,0,0}, {0,0,1}, {1,1,1}},  // rotation 2
    {{0,1,0}, {1,1,0}, {0,1,0}},  // rotation 3
};

// S piece - 3x3
static const int S_SHAPES[4][3][3] = {
    {{0,1,1}, {1,1,0}, {0,0,0}},  // rotation 0
    {{0,1,0}, {0,1,1}, {0,0,0}},  // rotation 1
    {{0,0,0}, {1,1,0}, {1,0,0}},  // rotation 2
    {{0,0,1}, {1,1,0}, {0,0,1}},  // rotation 3
};

// Z piece - 3x3
static const int Z_SHAPES[4][3][3] = {
    {{1,1,0}, {0,1,1}, {0,0,0}},  // rotation 0
    {{0,0,1}, {0,1,1}, {0,0,0}},  // rotation 1
    {{0,0,0}, {1,1,0}, {1,0,0}},  // rotation 2
    {{0,0,1}, {1,1,0}, {0,0,1}},  // rotation 3
};

// J piece - 3x3
static const int J_SHAPES[4][3][3] = {
    {{1,0,0}, {1,1,1}, {0,0,0}},  // rotation 0
    {{0,1,1}, {0,1,1}, {0,0,0}},  // rotation 1
    {{0,0,0}, {1,1,0}, {1,0,0}},  // rotation 2
    {{0,0,1}, {1,1,0}, {1,0,0}},  // rotation 3
};

// L piece - 3x3
static const int L_SHAPES[4][3][3] = {
    {{0,0,1}, {1,1,1}, {0,0,0}},  // rotation 0
    {{1,1,0}, {1,1,0}, {0,0,0}},  // rotation 1
    {{0,0,0}, {1,1,0}, {1,0,0}},  // rotation 2
    {{0,1,0}, {1,1,0}, {0,0,1}},  // rotation 3
};

// Get tetromino shape data based on type and rotation
// Returns a pointer to a 2D array (constant)
static inline const int *get_shape_ptr(tetromino_t type)
{
    switch (type) {
        case TETROMINO_I: return (const int (*))I_SHAPES;
        case TETROMINO_O: return (const int (*))O_SHAPES;
        case TETROMINO_T: return (const int (*))T_SHAPES;
        case TETROMINO_S: return (const int (*))S_SHAPES;
        case TETROMINO_Z: return (const int (*))Z_SHAPES;
        case TETROMINO_J: return (const int (*))J_SHAPES;
        case TETROMINO_L: return (const int (*))L_SHAPES;
        default: return (const int (*))I_SHAPES;
    }
}

// Get tetromino color based on type
static inline tetromino_color get_color(tetromino_t type) {
    switch (type) {
        case TETROMINO_I: return COLOR_I;
        case TETROMINO_O: return COLOR_O;
        case TETROMINO_T: return COLOR_T;
        case TETROMINO_S: return COLOR_S;
        case TETROMINO_Z: return COLOR_Z;
        case TETROMINO_J: return COLOR_J;
        case TETROMINO_L: return COLOR_L;
        default: return COLOR_I;
    }
}

// Get tetromino name string
static inline const char* get_name(tetromino_t type) {
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

// Get tetromino symbol character for display
static inline char get_symbol(tetromino_t type) {
    switch (type) {
        case TETROMINO_I: return '|';
        case TETROMINO_O: return '+';
        case TETROMINO_T: return 'T';
        case TETROMINO_S: return 'S';
        case TETROMINO_Z: return 'Z';
        case TETROMINO_J: return 'J';
        case TETROMINO_L: return 'L';
        default: return '|';
    }
}

// Create a new tetromino with given type and rotation
tetromino_t create_tetromino(tetromino_t type, rotation rotation);

// Rotate tetromino 90 degrees clockwise
void rotate_tetromino(tetromino_t* tet);

// Get tetromino bounding box dimensions
void get_tetromino_bounds(const tetromino_t* tet, int* x, int* y, int* width, int* height);

// Check if position is within tetromino
bool tetromino_has_block(const tetromino_t* tet, int dx, int dy);

// Get tetromino type from name
tetromino_t get_type_from_name(const char* name);

#endif // TETROMINO_H
