#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

// Input types
typedef enum {
    INPUT_NONE,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_ROTATE,
    INPUT_HARD_DROP,
    INPUT_PAUSE
} input_type;

// Input event structure
typedef struct {
    input_type type;
    time_t timestamp;
} input_event_t;

// Get current input
input_type get_input(void);

// Handle input (process and return action)
// Returns true if an action was taken, false if no input or paused
bool handle_input(void (*action)(void), bool* game_over);

// Initialize input handling
void input_init(void);

// Cleanup input handling
void input_cleanup(void);

// Wait for input
void input_wait(void);

// Get key code from ncurses
int input_get_keycode(void);

// Convert ncurses key to input type
input_type input_convert_key(int keycode);

// Check if input is valid (not a pause key)
bool input_is_valid(input_type type);

// Get action for input type
void input_get_action(input_type type, void (*action)(void));

#endif // INPUT_H
