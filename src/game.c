#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Initialize game
void game_init(game_t* game, const char* title, const char* player_name) {
    game_reset(game);
    if (title) {
        strncpy(game->title, title, sizeof(game->title) - 1);
        game->title[sizeof(game->title) - 1] = '\0';
    } else {
        strcpy(game->title, "Cursed Tetris");
    }
    if (player_name) {
        strncpy(game->player_name, player_name, sizeof(game->player_name) - 1);
        game->player_name[sizeof(game->player_name) - 1] = '\0';
    } else {
        strcpy(game->player_name, "Player");
    }
    game->state = STATE_PLAYING;
    game->running = true;
    game->drop_interval = 500;  // ms
    game->drop_count = 0;
    game->last_time.tv_sec = 0;
    game->last_time.tv_nsec = 0;
    game_set_next_piece(game, TETROMINO_COUNT - 1);
}

void game_reset(game_t* game) {
    board_init(&game->board);
    game_set_next_piece(game, TETROMINO_COUNT - 1);
    game->state = STATE_PLAYING;
    game->running = true;
}

void game_start(game_t* game) {
    game_reset(game);
    game_set_next_piece(game, TETROMINO_COUNT - 1);
    game->current_piece = create_tetromino(game_get_next_piece(game), ROTATION_0);
    game_set_next_piece(game, board_get_next_type(&game->board));
    game->drop_count = 0;
    clock_gettime(CLOCK_REALTIME, &game->last_time);
}

void game_update(game_t* game) {
    if (!game->running) {
        return;
    }

    time_t now = time(NULL);
    long elapsed_ms = (now - game->last_time.tv_sec) * 1000 +
                      (game->last_time.tv_nsec / 1000000L);

    if (elapsed_ms >= game->drop_interval) {
        game->drop_count++;
        game->last_time.tv_sec = now;
        game->last_time.tv_nsec = time(NULL) * 1000 - now * 1000;

        if (game->drop_count >= 10) {
            game->drop_count = 0;
            game_move_down(game);
        }
    }
}

void game_move_down(game_t* game) {
    if (board_move_down(&game->board, game->current_piece.x, game->current_piece.y)) {
        game->current_piece.y++;
    } else {
        board_place(&game->board, game->current_piece.x, game->current_piece.y,
                    get_symbol(game->current_piece.type), game->current_piece.color);
        game->current_piece = create_tetromino(game_get_next_piece(game), ROTATION_0);
        game_set_next_piece(game, board_get_next_type(&game->board));

        board_clear_lines(&game->board);

        if (board_is_game_over(&game->board)) {
            game->state = STATE_GAMEOVER;
            game->running = false;
            sprintf(game->message, "Game Over! Score: %d", game_get_score(game));
        }
    }
}

void game_handle_input(game_t* game) {
    if (game->state == STATE_PAUSED) {
        game_action_valid(game, INPUT_PAUSE) ?
            (game->state = STATE_PLAYING) :
            (game->state = STATE_GAMEOVER);
        return;
    }

    if (game->state == STATE_GAMEOVER) {
        if (game_action_valid(game, INPUT_PAUSE)) {
            game->state = STATE_PAUSED;
        }
        return;
    }

    if (game_action_valid(game, INPUT_NONE)) {
        return;
    }

    switch (game->state) {
        case STATE_PLAYING:
            switch (game->current_piece.type) {
                case TETROMINO_I:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    else if (game_action_valid(game, INPUT_HARD_DROP)) action_hard_drop(game);
                    break;
                case TETROMINO_O:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    else if (game_action_valid(game, INPUT_HARD_DROP)) action_hard_drop(game);
                    break;
                case TETROMINO_T:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    break;
                case TETROMINO_S:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    break;
                case TETROMINO_Z:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    break;
                case TETROMINO_J:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    break;
                case TETROMINO_L:
                    if (game_action_valid(game, INPUT_LEFT)) action_move_left(game);
                    else if (game_action_valid(game, INPUT_RIGHT)) action_move_right(game);
                    else if (game_action_valid(game, INPUT_DOWN)) action_move_down(game);
                    else if (game_action_valid(game, INPUT_ROTATE)) action_rotate(game);
                    break;
            }
            break;
    }
}

void action_move_left(game_t* game) {
    tetromino_t* piece = &game->current_piece;
    if (piece->x > 0 && board_collision(&game->board, piece->x - 1, piece->y, piece->shape_width, piece->shape_height)) {
        return;
    }
    piece->x--;
}

void action_move_right(game_t* game) {
    tetromino_t* piece = &game->current_piece;
    if (piece->x + piece->shape_width >= BOARD_WIDTH &&
        board_collision(&game->board, piece->x + 1, piece->y, piece->shape_width, piece->shape_height)) {
        return;
    }
    piece->x++;
}

void action_move_down(game_t* game) {
    game_move_down(game);
}

void action_rotate(game_t* game) {
    tetromino_t* piece = &game->current_piece;
    const int (*shape_ptr)() = get_shape_ptr(piece->type);

    // Try rotating
    piece->rotation = (piece->rotation + 1) % 4;
    piece->shape_width = piece->shape_height;
    piece->shape_height = piece->shape_width;
    if (piece->shape_width > piece->shape_height) {
        int temp = piece->shape_width;
        piece->shape_width = piece->shape_height;
        piece->shape_height = temp;
    }

    // Wall kick - try different positions if rotation causes collision
    if (board_collision(&game->board, piece->x, piece->y, piece->shape_width, piece->shape_height)) {
        // Try moving left
        if (piece->x > 0 &&
            board_collision(&game->board, piece->x - 1, piece->y, piece->shape_width, piece->shape_height)) {
            return;
        }
        piece->x--;
        if (board_collision(&game->board, piece->x, piece->y, piece->shape_width, piece->shape_height)) {
            // Revert
            piece->rotation = (piece->rotation - 1 + 4) % 4;
            piece->shape_width = piece->shape_height;
            piece->shape_height = piece->shape_width;
            if (piece->shape_width > piece->shape_height) {
                int temp = piece->shape_width;
                piece->shape_width = piece->shape_height;
                piece->shape_height = temp;
            }
        }
    }
}

void action_hard_drop(game_t* game) {
    tetromino_t* piece = &game->current_piece;
    while (board_collision(&game->board, piece->x, piece->y + 1, piece->shape_width, piece->shape_height)) {
        piece->y++;
    }
    board_place(&game->board, piece->x, piece->y, get_symbol(piece->type), piece->color);
    game->current_piece = create_tetromino(game_get_next_piece(game), ROTATION_0);
    game_set_next_piece(game, board_get_next_type(&game->board));
    board_clear_lines(&game->board);

    if (board_is_game_over(&game->board)) {
        game->state = STATE_GAMEOVER;
        game->running = false;
        sprintf(game->message, "Game Over! Score: %d", game_get_score(game));
    }
}

void action_pause(game_t* game) {
    if (game->state == STATE_PLAYING) {
        game->state = STATE_PAUSED;
    } else if (game->state == STATE_PAUSED) {
        game->state = STATE_PLAYING;
    }
}

bool game_action_valid(const game_t* game, input_type type) {
    if (game->state == STATE_PAUSED) {
        return type == INPUT_PAUSE;
    }
    if (game->state == STATE_GAMEOVER) {
        return type == INPUT_PAUSE;
    }
    return true;
}

game_state game_get_state(const game_t* game) {
    return game->state;
}

int game_get_score(const game_t* game) {
    return game->board.score;
}

int game_get_lines(const game_t* game) {
    return game->board.lines_cleared;
}

int game_get_level(const game_t* game) {
    return game->board.level;
}

void game_cleanup(game_t* game) {
    // ncurses cleanup
    napms(0);
    endwin();
    // Free any allocated memory
    game_set_next_piece(game, TETROMINO_COUNT - 1);
}
