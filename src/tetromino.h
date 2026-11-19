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
} tetromino_type;

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
    {{0,0,1}, {1,1,1}, {0,0,0}},  // rotation 2
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

// Shape matrix pointers
static const int (*I_SHAPES_PTR)[4][4] = I_SHAPES;
static const int (*O_SHAPES_PTR)[3][3] = O_SHAPES;
static const int (*T_SHAPES_PTR)[3][3] = T_SHAPES;
static const int (*S_SHAPES_PTR)[3][3] = S_SHAPES;
static const int (*Z_SHAPES_PTR)[3][3] = Z_SHAPES;
static const int (*J_SHAPES_PTR)[3][3] = J_SHAPES;
static const int (*L_SHAPES_PTR)[3][3] = L_SHAPES;

// Tetromino structure
typedef struct {
    tetromino_type type;
    rotation rotation;
    tetromino_color color;
    int shape_width;
    int shape_height;
} tetromino_t;

// Get tetromino shape data based on type and rotation
// Returns a pointer to a 2D array
static inline int (*get_shape_ptr(tetromino_type type))() {
    switch (type) {
        case TETROMINO_I: return (int (*)[4][4])I_SHAPES_PTR;
        case TETROMINO_O: return (int (*)[3][3])O_SHAPES_PTR;
        case TETROMINO_T: return (int (*)[3][3])T_SHAPES_PTR;
        case TETROMINO_S: return (int (*)[3][3])S_SHAPES_PTR;
        case TETROMINO_Z: return (int (*)[3][3])Z_SHAPES_PTR;
        case TETROMINO_J: return (int (*)[3][3])J_SHAPES_PTR;
        case TETROMINO_L: return (int (*)[3][3])L_SHAPES_PTR;
        default: return (int (*)[4][4])I_SHAPES_PTR;
    }
}

// Get tetromino color based on type
static inline tetromino_color get_color(tetromino_type type) {
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
static inline const char* get_name(tetromino_type type) {
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
static inline char get_symbol(tetromino_type type) {
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
tetromino_t create_tetromino(tetromino_type type, rotation rotation);

// Rotate tetromino 90 degrees clockwise
void rotate_tetromino(tetromino_t* tet);

// Get tetromino bounding box dimensions
void get_tetromino_bounds(const tetromino_t* tet, int* x, int* y, int* width, int* height);

// Check if position is within tetromino
bool tetromino_has_block(const tetromino_t* tet, int dx, int dy);

// Get tetromino type from name
tetromino_type get_type_from_name(const char* name);

#endif // TETROMINO_H
