#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Classic line-clear scores (x1..x4), multiplied by the current level. */
static const int LINE_SCORES[5] = { 0, 100, 300, 500, 800 };

/* Simplified SRS wall kicks: try the plain position first, then shift
 * the piece left/right by 1 and 2 cells. */
static const int KICK_OFFSETS[5] = { 0, -1, 1, -2, 2 };

static void copy_str(char* dst, size_t n, const char* src)
{
    if (src == NULL)
        src = "";
    strncpy(dst, src, n - 1);
    dst[n - 1] = '\0';
}

/* Guideline gravity: (0.8 - (level-1) * 0.007) ^ (level-1) seconds. */
static int gravity_ms(int level)
{
    double t = 1.0;
    double base = 0.8 - (level - 1) * 0.007;
    for (int i = 1; i < level; i++)
        t *= base;
    int ms = (int)(t * 1000.0 + 0.5);
    return ms > 0 ? ms : 1;
}

static void refill_bag(game_t* game)
{
    for (int i = 0; i < TETROMINO_COUNT; i++)
        game->bag[i] = (tetromino_type)i;
    for (int i = TETROMINO_COUNT - 1; i > 0; i--) { /* Fisher-Yates */
        int j = rand() % (i + 1);
        tetromino_type tmp = game->bag[i];
        game->bag[i] = game->bag[j];
        game->bag[j] = tmp;
    }
    game->bag_index = 0;
}

static tetromino_type next_from_bag(game_t* game)
{
    if (game->bag_index >= TETROMINO_COUNT)
        refill_bag(game);
    return game->bag[game->bag_index++];
}

static void fill_preview(game_t* game)
{
    for (int i = 0; i < NEXT_PREVIEW_COUNT; i++)
        game->next_pieces.pieces[i] =
            create_tetromino(next_from_bag(game), ROTATION_0, 0, 0);
}

/* Shift the preview and add a fresh piece at the back. */
static void refresh_preview(game_t* game)
{
    for (int i = 0; i + 1 < NEXT_PREVIEW_COUNT; i++)
        game->next_pieces.pieces[i] = game->next_pieces.pieces[i + 1];
    game->next_pieces.pieces[NEXT_PREVIEW_COUNT - 1] =
        create_tetromino(next_from_bag(game), ROTATION_0, 0, 0);
}

static void spawn_piece(game_t* game)
{
    game->current_piece = create_tetromino(
        game->next_pieces.pieces[0].type, ROTATION_0,
        PIECE_SPAWN_X, PIECE_SPAWN_Y);
    refresh_preview(game);
}

static void lock_and_spawn(game_t* game)
{
    board_lock(&game->board, &game->current_piece);

    int cleared = board_clear_lines(&game->board);
    if (cleared > 0) {
        game->board.lines_cleared += cleared;
        game->board.score += LINE_SCORES[cleared] * game->board.level;
        game->board.level = game->board.lines_cleared / 10 + 1;
        game->drop_interval = gravity_ms(game->board.level);
    }

    spawn_piece(game);
    clock_gettime(CLOCK_MONOTONIC, &game->last_drop);

    /* Game over: the new piece cannot fit at its spawn position.
     * (score and lines stay visible in the side panel) */
    if (board_check(&game->board, &game->current_piece)) {
        game->state = STATE_GAMEOVER;
        snprintf(game->message, sizeof(game->message),
                 "GAME OVER (r=again)");
    }
}

void game_init(game_t* game, const char* title, const char* player_name)
{
    memset(game, 0, sizeof(*game));
    copy_str(game->title, sizeof(game->title), title);
    if (game->title[0] == '\0')
        copy_str(game->title, sizeof(game->title), "Cursed Tetris");
    copy_str(game->player_name, sizeof(game->player_name), player_name);
    if (game->player_name[0] == '\0')
        copy_str(game->player_name, sizeof(game->player_name), "Player");
    srand((unsigned)time(NULL));
}

void game_reset(game_t* game)
{
    board_init(&game->board);
    game->drop_interval = gravity_ms(1);
    game->bag_index = TETROMINO_COUNT; /* force a fresh bag */
    fill_preview(game);
    game->state = STATE_PLAYING;
    game->running = true;
    game->message[0] = '\0';
}

void game_start(game_t* game)
{
    game_reset(game);
    spawn_piece(game);
    clock_gettime(CLOCK_MONOTONIC, &game->last_drop);
}

void game_update(game_t* game)
{
    if (game->state != STATE_PLAYING || !game->running)
        return;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long elapsed_ms = (now.tv_sec - game->last_drop.tv_sec) * 1000L +
                      (now.tv_nsec - game->last_drop.tv_nsec) / 1000000L;

    if (elapsed_ms >= game->drop_interval) {
        if (!board_check_offset(&game->board, &game->current_piece, 0, 1)) {
            game->current_piece.y++;
        } else {
            lock_and_spawn(game);
        }
        clock_gettime(CLOCK_MONOTONIC, &game->last_drop);
    }
}

void game_handle_input(game_t* game, input_type input)
{
    if (!game->running)
        return;

    switch (game->state) {
    case STATE_GAMEOVER:
        if (input == INPUT_QUIT) {
            game->running = false;
        } else if (input == INPUT_RESTART) {
            game_start(game);
        }
        break;

    case STATE_PAUSED:
        if (input == INPUT_PAUSE)
            action_pause(game);
        else if (input == INPUT_QUIT)
            game->running = false;
        break;

    case STATE_PLAYING:
        switch (input) {
        case INPUT_LEFT:
            action_move_left(game);
            break;
        case INPUT_RIGHT:
            action_move_right(game);
            break;
        case INPUT_DOWN:
            action_move_down(game);
            break;
        case INPUT_ROTATE_CW:
            action_rotate_cw(game);
            break;
        case INPUT_ROTATE_CCW:
            action_rotate_ccw(game);
            break;
        case INPUT_HARD_DROP:
            action_hard_drop(game);
            break;
        case INPUT_PAUSE:
            action_pause(game);
            break;
        case INPUT_QUIT:
            game->running = false;
            break;
        default:
            break;
        }
        break;
    }
}

void action_move_left(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;
    if (!board_check_offset(&game->board, &game->current_piece, -1, 0))
        game->current_piece.x--;
}

void action_move_right(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;
    if (!board_check_offset(&game->board, &game->current_piece, 1, 0))
        game->current_piece.x++;
}

void action_move_down(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;
    if (!board_check_offset(&game->board, &game->current_piece, 0, 1)) {
        game->current_piece.y++;
        game->board.score += 1; /* soft drop bonus */
        clock_gettime(CLOCK_MONOTONIC, &game->last_drop);
    } else {
        lock_and_spawn(game);
    }
}

/* Rotate 90 degrees in the given direction (+1 = CW, -1 = CCW),
 * trying the kick offsets in order. */
static void rotate_to(game_t* game, int direction)
{
    piece_t rotated = game->current_piece;
    int new_rot = rotated.rotation + direction;
    new_rot = ((new_rot % ROTATION_COUNT) + ROTATION_COUNT) % ROTATION_COUNT;
    rotated.rotation = (rotation)new_rot;

    for (int i = 0; i < (int)(sizeof(KICK_OFFSETS) / sizeof(KICK_OFFSETS[0])); i++) {
        int kick = KICK_OFFSETS[i];
        if (!board_check_offset(&game->board, &rotated, kick, 0)) {
            rotated.x += kick;
            game->current_piece = rotated;
            return;
        }
    }
    /* No kick position fit: rotation is rejected. */
}

void action_rotate_cw(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;
    rotate_to(game, +1);
}

void action_rotate_ccw(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;
    rotate_to(game, -1);
}

void action_hard_drop(game_t* game)
{
    if (game->state != STATE_PLAYING)
        return;

    int distance = 0;
    while (!board_check_offset(&game->board, &game->current_piece, 0, 1)) {
        game->current_piece.y++;
        distance++;
    }
    game->board.score += 2 * distance; /* hard drop bonus */
    lock_and_spawn(game);
}

void action_pause(game_t* game)
{
    if (game->state == STATE_PLAYING) {
        game->state = STATE_PAUSED;
        snprintf(game->message, sizeof(game->message),
                 "PAUSED (p = resume)");
    } else if (game->state == STATE_PAUSED) {
        game->state = STATE_PLAYING;
        game->message[0] = '\0';
        clock_gettime(CLOCK_MONOTONIC, &game->last_drop);
    }
}

game_state game_get_state(const game_t* game)
{
    return game->state;
}

int game_get_score(const game_t* game)
{
    return game->board.score;
}

int game_get_lines(const game_t* game)
{
    return game->board.lines_cleared;
}

int game_get_level(const game_t* game)
{
    return game->board.level;
}

int game_get_next_piece(const game_t* game)
{
    return game->next_pieces.pieces[0].type;
}

bool game_is_running(const game_t* game)
{
    return game->running;
}

bool game_is_paused(const game_t* game)
{
    return game->state == STATE_PAUSED;
}

bool game_is_over(const game_t* game)
{
    return game->state == STATE_GAMEOVER;
}
