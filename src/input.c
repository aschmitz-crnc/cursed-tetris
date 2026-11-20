#include "input.h"

input_type input_convert_key(int keycode)
{
    switch (keycode) {
    case KEY_LEFT:
    case 'h':
    case 'H':
        return INPUT_LEFT;
    case KEY_RIGHT:
    case 'l':
    case 'L':
        return INPUT_RIGHT;
    case KEY_DOWN:
    case 'm':
    case 'M':
        return INPUT_DOWN;
    case KEY_UP:
    case 'j':
    case 'J':
        return INPUT_ROTATE_CW;
    case 'k':
    case 'K':
        return INPUT_ROTATE_CCW;
    case ' ':
        return INPUT_HARD_DROP;
    case 'p':
    case 'P':
        return INPUT_PAUSE;
    case 'q':
    case 'Q':
        return INPUT_QUIT;
    case 'r':
    case 'R':
        return INPUT_RESTART;
    default:
        return INPUT_NONE;
    }
}

input_type input_wait(void)
{
    int keycode = getch();
    if (keycode == ERR)
        return INPUT_NONE;
    return input_convert_key(keycode);
}
