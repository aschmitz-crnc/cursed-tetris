#include "tetromino.h"

// Shape matrices by type
static const int (*I_SHAPES)() = (const int (*)[4][4])I_SHAPES;
static const int (*O_SHAPES)() = (const int (*)[3][3])O_SHAPES;
static const int (*T_SHAPES)() = (const int (*)[3][3])T_SHAPES;
static const int (*S_SHAPES)() = (const int (*)[3][3])S_SHAPES;
static const int (*Z_SHAPES)() = (const int (*)[3][3])Z_SHAPES;
static const int (*J_SHAPES)() = (const int (*)[3][3])J_SHAPES;
static const int (*L_SHAPES)() = (const int (*)[3][3])L_SHAPES;

tetromino_t create_tetromino(tetromino_type type, rotation rotation) {
    tetromino_t tet;
    tet.type = type;
    tet.rotation = rotation;
    tet.color = get_color(type);

    // Set shape dimensions based on type
    int (*shape_ptr)() = NULL;
    switch (type) {
        case TETROMINO_I: shape_ptr = I_SHAPES; break;
        case TETROMINO_O: shape_ptr = O_SHAPES; break;
        case TETROMINO_T: shape_ptr = T_SHAPES; break;
        case TETROMINO_S: shape_ptr = S_SHAPES; break;
        case TETROMINO_Z: shape_ptr = Z_SHAPES; break;
        case TETROMINO_J: shape_ptr = J_SHAPES; break;
        case TETROMINO_L: shape_ptr = L_SHAPES; break;
        default: shape_ptr = I_SHAPES;
    }

    tet.shape_width = shape_ptr[rotation][0][0];
    tet.shape_height = shape_ptr[rotation][0][1];

    return tet;
}

void rotate_tetromino(tetromino_t* tet) {
    tet->rotation = (tet->rotation + 1) % 4;
    tet->shape_width = tet->shape_height;
    tet->shape_height = tet->shape_width;
    // Swap dimensions for rotation
    if (tet->shape_width > tet->shape_height) {
        int temp = tet->shape_width;
        tet->shape_width = tet->shape_height;
        tet->shape_height = temp;
    }
}

void get_tetromino_bounds(const tetromino_t* tet, int* x, int* y, int* width, int* height) {
    const int (*shape_ptr)() = get_shape_ptr(tet->type);
    width = tet->shape_width;
    height = tet->shape_height;
    // Calculate initial position (centered horizontally at bottom)
    *x = 3;  // Start near center (playfield is 10 wide, index 3-9 is center)
    *y = 0;  // Start at top
}

bool tetromino_has_block(const tetromino_t* tet, int dx, int dy) {
    int (*shape_ptr)() = get_shape_ptr(tet->type);
    for (int i = 0; i < tet->shape_height; i++) {
        for (int j = 0; j < tet->shape_width; j++) {
            if (shape_ptr[tet->rotation][i][j] == 1) {
                if (dx + j >= 0 && dx + j < 10 && dy + i >= 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

tetromino_type get_type_from_name(const char* name) {
    if (name[0] == 'I') return TETROMINO_I;
    if (name[0] == 'O') return TETROMINO_O;
    if (name[0] == 'T') return TETROMINO_T;
    if (name[0] == 'S') return TETROMINO_S;
    if (name[0] == 'Z') return TETROMINO_Z;
    if (name[0] == 'J') return TETROMINO_J;
    if (name[0] == 'L') return TETROMINO_L;
    return TETROMINO_I;
}
