#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "tetromino.h"
#include "input.h"
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Time structure for tracking game time
struct timespec game_time;

// Screen layout constants
#define PLAYFIELD_WIDTH 40
#define PLAYFIELD_HEIGHT 20
#define SCORE_WIDTH 20
#define SCORE_HEIGHT 6

// Color pairs
#define COLOR_PAIR_BACKGROUND 1
#define COLOR_PAIR_TITLE 2
#define COLOR_PAIR_SCORE 3
#define COLOR_PAIR_NEXT 4
#define COLOR_PAIR_MESSAGE 5
#define COLOR_PAIR_GAMEOVER 6

// Game states
typedef enum {
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER
} game_state;

// Tetrominoes for next preview
typedef struct {
    tetromino_t pieces[3];
    int current_index;
} next_pieces_t;

// Game structure
typedef struct {
    board_t board;
    tetromino_t current_piece;
    next_pieces_t next_pieces;
    game_state state;
    bool running;
    int drop_interval;
    int drop_count;
    char title[32];
    char player_name[32];
    char message[64];
} game_t;

// Initialize game
void game_init(game_t* game, const char* title, const char* player_name);

// Reset game
void game_reset(game_t* game);

// Start game
void game_start(game_t* game);

// Update game state
void game_update(game_t* game);

// Render game
void game_render(game_t* game);

// Handle input
void game_handle_input(game_t* game);

// Handle specific actions
void action_move_left(game_t* game);
void action_move_right(game_t* game);
void action_move_down(game_t* game);
void action_rotate(game_t* game);
void action_hard_drop(game_t* game);
void action_pause(game_t* game);

// Check if action is valid
bool game_action_valid(const game_t* game, input_type type);

// Get game state
game_state game_get_state(const game_t* game);

// Get game statistics
int game_get_score(const game_t* game);
int game_get_lines(const game_t* game);
int game_get_level(const game_t* game);

// Render components
void game_render_title(game_t* game);
void game_render_header(game_t* game);
void game_render_playfield(game_t* game);
void game_render_next_pieces(game_t* game);
void game_render_score(game_t* game);
void game_render_message(game_t* game);

// Render based on state
void game_render_playing(game_t* game);
void game_render_paused(game_t* game);
void game_render_gameover(game_t* game);

// Render all components
void game_render_all(game_t* game);

// Set next piece type
void game_set_next_piece(game_t* game, int type);

// Get next piece type
int game_get_next_piece(game_t* game);

// Check if game is running
bool game_is_running(const game_t* game);

// Check if game is paused
bool game_is_paused(const game_t* game);

// Check if game is over
bool game_is_over(const game_t* game);

// Cleanup game
void game_cleanup(game_t* game);

#endif // GAME_H
