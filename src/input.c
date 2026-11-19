#include "input.h"
#include <unistd.h>

// Action functions (defined in game.c)
static void (*input_action_map[7])(void) = {
    NULL,        // INPUT_NONE
    action_move_left,
    action_move_right,
    action_move_down,
    action_rotate,
    action_hard_drop,
    action_pause
};

void input_init(void) {
    // ncurses should already be initialized by caller
}

void input_cleanup(void) {
    // ncurses should already be cleaned up by caller
}

void input_wait(void) {
    if (input_get_keycode() != KEY_NULL) {
        input_type type = input_convert_key(input_get_keycode());
        if (input_is_valid(type)) {
            input_action_map[type]();
        }
    }
}

int input_get_keycode(void) {
    return getch();
}

input_type input_convert_key(int keycode) {
    if (keycode == KEY_LEFT || keycode == 'h' || keycode == 'H') {
        return INPUT_LEFT;
    } else if (keycode == KEY_RIGHT || keycode == 'l' || keycode == 'L') {
        return INPUT_RIGHT;
    } else if (keycode == KEY_DOWN || keycode == 'j' || keycode == 'J' || keycode == 'k' || keycode == 'K') {
        return INPUT_DOWN;
    } else if (keycode == KEY_UP || keycode == 'i' || keycode == 'I') {
        return INPUT_ROTATE;
    } else if (keycode == ' ' || keycode == 32) {
        return INPUT_HARD_DROP;
    } else if (keycode == 'p' || keycode == 'P' || keycode == KEY_P) {
        return INPUT_PAUSE;
    }
    return INPUT_NONE;
}

bool input_is_valid(input_type type) {
    return type != INPUT_PAUSE;
}

void input_get_action(input_type type, void (*action)(void)) {
    if (type != INPUT_NONE) {
        action();
    }
}
