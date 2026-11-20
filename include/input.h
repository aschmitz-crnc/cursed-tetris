#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

/* Input events delivered to the game loop */
typedef enum {
    INPUT_NONE,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_ROTATE_CW,
    INPUT_ROTATE_CCW,
    INPUT_HARD_DROP,
    INPUT_PAUSE,
    INPUT_QUIT,
    INPUT_RESTART
} input_type;

/* Non-blocking poll: return the next key event, or INPUT_NONE if
 * nothing is waiting.  Requires initscr(), keypad() and nodelay(). */
input_type input_wait(void);

/* Map an ncurses keycode to an input event. */
input_type input_convert_key(int keycode);

#endif /* INPUT_H */
