#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "tetromino.h"
#include "input.h"
#include <stdbool.h>
#include <time.h>

#define GAME_MESSAGE_LEN 64

/* Game phases */
typedef enum {
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER
} game_state;

/* Number of upcoming pieces shown in the preview */
#define NEXT_PREVIEW_COUNT 3

typedef struct {
    piece_t pieces[NEXT_PREVIEW_COUNT]; /* pieces[0] spawns next */
} next_pieces_t;

typedef struct {
    board_t board;
    piece_t current_piece;
    next_pieces_t next_pieces;

    game_state state;
    bool running;               /* app loop flag (false only on quit)  */

    int drop_interval;          /* ms per gravity step, level-based   */
    struct timespec last_drop;  /* monotonic clock of last gravity    */

    /* 7-bag randomizer */
    tetromino_type bag[TETROMINO_COUNT];
    int bag_index;

    char title[32];
    char player_name[32];
    char message[GAME_MESSAGE_LEN];
} game_t;

/* Basic init (fills in title/player, seeds the RNG). */
void game_init(game_t* game, const char* title, const char* player_name);

/* Clear the board and preview; game is ready for game_start(). */
void game_reset(game_t* game);

/* Reset (if needed) and spawn the first piece. */
void game_start(game_t* game);

/* Advance gravity.  Call once per frame. */
void game_update(game_t* game);

/* Apply one input event to the game. */
void game_handle_input(game_t* game, input_type input);

/* Player actions (all no-ops outside STATE_PLAYING, except pause). */
void action_move_left(game_t* game);
void action_move_right(game_t* game);
void action_move_down(game_t* game);
void action_rotate(game_t* game);
void action_hard_drop(game_t* game);
void action_pause(game_t* game);

/* Accessors */
game_state game_get_state(const game_t* game);
int game_get_score(const game_t* game);
int game_get_lines(const game_t* game);
int game_get_level(const game_t* game);
int game_get_next_piece(const game_t* game);
bool game_is_running(const game_t* game);
bool game_is_paused(const game_t* game);
bool game_is_over(const game_t* game);

#endif /* GAME_H */
