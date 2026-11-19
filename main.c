#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

// Define minimum terminal size
#define MIN_TERMINAL_WIDTH 80
#define MIN_TERMINAL_HEIGHT 25

// Time structure for tracking game time
struct timespec game_time;

// Global game pointer
static game_t* g_game = NULL;

// Signal handler for cleanup
static void signal_handler(int sig) {
    if (g_game) {
        game_cleanup(g_game);
        g_game = NULL;
    }
    if (sig == SIGINT) {
        exit(0);
    }
}

// Render ASCII art header
static void render_header(void) {
    const char* title = "       ____  ____ _        ";
    const char* sub1 = " /    /  \\  /    \\ \\      / ";
    const char* sub2 = "/____/___/\\_____/\\_____/  ";
    const char* credits = "    A Cursed Tetris Game\n";
    const char* controls = "    Left:  h/H    Right:  l/L    Down:  j/J    Rotate: i/I\n";
    printw("%s\n%s\n%s\n%s\n", title, sub1, sub2, credits);
}

// Render playfield
static void render_playfield(const game_t* game) {
    int i, j;

    // Calculate position based on terminal size
    // Playfield: 40 chars wide, 20 rows high
    // Center horizontally, left-aligned
    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);

    int start_x = (max_x - PLAYFIELD_WIDTH) / 2;
    int start_y = 3;  // Leave room for header

    // Ensure minimum size
    if (start_x < 1) start_x = 1;
    int end_x = start_x + PLAYFIELD_WIDTH;
    int end_y = start_y + PLAYFIELD_HEIGHT;

    // Clear area
    for (i = start_y; i < end_y; i++) {
        for (j = start_x; j < end_x; j++) {
            mvaddch(i, j, ' ');
        }
    }

    // Render board
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            int cell = game->board.grid[j][i];
            char ch = ' ';

            if (cell > 0) {
                // Determine character based on cell value
                int lower = cell & 0xFF;

                switch ((cell >> 8) & 0xFF) {
                    case '|': ch = '|'; break;
                    case '+': ch = '+'; break;
                    case 'T': ch = 'T'; break;
                    case 'S': ch = 'S'; break;
                    case 'Z': ch = 'Z'; break;
                    case 'J': ch = 'J'; break;
                    case 'L': ch = 'L'; break;
                    default:  ch = ' '; break;
                }
            }

            mvaddch(start_y + i, start_x + j, ch);
        }
    }
}

// Render next pieces preview
static void render_next_pieces(const game_t* game) {
    int i, j, k;

    // Calculate position - right side of screen
    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);

    int start_x = max_x - PLAYFIELD_WIDTH - 2;
    int start_y = 2;

    // Ensure minimum size
    if (start_x < PLAYFIELD_WIDTH + 10) start_x = PLAYFIELD_WIDTH + 10;
    int end_x = start_x + PLAYFIELD_WIDTH;
    int end_y = start_y + 3;

    // Clear area
    for (i = start_y; i < end_y; i++) {
        for (j = start_x; j < end_x; j++) {
            mvaddch(i, j, ' ');
        }
    }

    // Draw title
    mvaddstr(start_y, start_x, "NEXT:");
    color_pair(start_y + 1, start_x, COLOR_PAIR_NEXT);

    // Draw pieces
    int piece_y = start_y + 2;
    for (i = 0; i < 3; i++) {
        tetromino_t piece = game->next_pieces.pieces[i];
        int (*shape_ptr)() = get_shape_ptr(piece.type);

        // Calculate visible portion
        int visible_width = piece.shape_width;
        int visible_height = piece.shape_height;

        for (j = 0; j < visible_height; j++) {
            for (k = 0; k < visible_width; k++) {
                if (shape_ptr[piece.rotation][j][k] == 1) {
                    mvaddch(piece_y + j, start_x + k, get_symbol(piece.type));
                    color_start(piece_y + j, start_x + k);
                    color_change(piece_y + j, start_x + k, piece.color);
                    color_end(piece_y + j, start_x + k);
                }
            }
        }
        piece_y += 2;
    }
}

// Render score box
static void render_score(const game_t* game) {
    int i;

    // Calculate position - right side of screen
    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);

    int start_x = max_x - SCORE_WIDTH - 2;
    int start_y = PLAYFIELD_HEIGHT + 2;

    // Ensure minimum size
    if (start_x < PLAYFIELD_WIDTH + 10 + SCORE_WIDTH + 2) start_x = PLAYFIELD_WIDTH + 10 + SCORE_WIDTH + 2;
    int end_x = start_x + SCORE_WIDTH;
    int end_y = start_y + SCORE_HEIGHT;

    // Clear area
    for (i = start_y; i < end_y; i++) {
        for (int j = start_x; j < end_x; j++) {
            mvaddch(i, j, ' ');
        }
    }

    // Draw title
    mvaddstr(start_y, start_x, "SCORE:");
    color_pair(start_y + 1, start_x, COLOR_PAIR_SCORE);

    // Draw values
    mvprintw(start_y + 1, start_x + 6, "Lines: %d", game_get_lines(game));
    mvprintw(start_y + 2, start_x + 6, "Level: %d", game_get_level(game));
    mvprintw(start_y + 3, start_x + 6, "Score: %d", game_get_score(game));
}

// Render message box
static void render_message(const game_t* game) {
    int start_x, start_y, end_x, end_y;

    // Calculate position - centered at bottom
    int max_x = getmaxx(stdscr);
    int max_y = getmaxy(stdscr);

    start_x = (max_x - strlen(game->message)) / 2;
    start_y = max_y - 3;

    // Ensure minimum size
    if (start_x < 1) start_x = 1;
    end_x = start_x + strlen(game->message);
    end_y = start_y + 3;

    if (end_y > max_y) end_y = max_y;

    // Clear area
    for (int i = start_y; i < end_y; i++) {
        for (int j = start_x; j < end_x; j++) {
            mvaddch(i, j, ' ');
        }
    }

    // Draw box
    mvaddch(start_y, start_x, '┌');
    mvaddch(end_y - 1, start_x, '└');
    mvaddch(start_y, end_x - 1, '┐');
    mvaddch(end_y - 1, end_x - 1, '┘');

    // Draw message
    mvaddstr(start_y + 1, start_x + 1, game->message);
}

// Render based on state
static void render_state(const game_t* game) {
    switch (game->state) {
        case STATE_PLAYING:
            render_title();
            render_header();
            render_playfield(game);
            render_next_pieces(game);
            render_score(game);
            break;
        case STATE_PAUSED:
            render_title();
            render_header();
            render_playfield(game);
            render_next_pieces(game);
            render_score(game);
            render_message(game);
            break;
        case STATE_GAMEOVER:
            render_title();
            render_header();
            render_playfield(game);
            render_next_pieces(game);
            render_score(game);
            render_message(game);
            break;
    }
}

// Render all components
static void render_all(const game_t* game) {
    render_title();
    render_header();
    render_playfield(game);
    render_next_pieces(game);
    render_score(game);
    render_message(game);
    render_state(game);
}

// Render title
static void render_title(void) {
    const char* title = "       ____  ____ _        ";
    const char* sub1 = " /    /  \\  /    \\ \\      / ";
    const char* sub2 = "/____/___/\\_____/\\_____/  ";
    const char* credits = "    A Cursed Tetris Game\n";
    printw("%s\n%s\n%s\n%s\n", title, sub1, sub2, credits);
}

// Initialize ncurses colors properly
static void init_ncurses_colors(void) {
    // Initialize default colors
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 255, 255, 255);
    init_color(COLOR_BLUE, 0, 128, 255);
    init_color(COLOR_GREEN, 0, 255, 0);
    init_color(COLOR_YELLOW, 255, 255, 0);
    init_color(COLOR_PURPLE, 128, 0, 128);
    init_color(COLOR_CYAN, 0, 255, 255);
    init_color(COLOR_RED, 255, 0, 0);
    init_color(COLOR_MAGENTA, 255, 0, 255);
    init_color(COLOR_ORANGE, 255, 128, 0);

    // Initialize color pairs
    init_pair(COLOR_PAIR_BACKGROUND, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_PAIR_TITLE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SCORE, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_NEXT, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MESSAGE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_GAMEOVER, COLOR_RED, COLOR_BLACK);
}

// Render game
void game_render(game_t* game) {
    init_ncurses_colors();

    // Check terminal size
    int height, width;
    getmaxyx(stdscr, height, width);

    if (height < MIN_TERMINAL_HEIGHT || width < MIN_TERMINAL_WIDTH) {
        mvprintw(MIN_TERMINAL_HEIGHT - 1, 0, "Terminal too small. Minimum: %dx%d\n",
                 MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
        napms(100);
        endwin();
        return;
    }

    // Clear screen
    clear();

    // Render based on state
    render_state(game);

    // Refresh screen
    refresh();
}

// Render all components (called during game loop)
void game_render_all(game_t* game) {
    // Clear screen
    clear();

    // Render title
    render_title();

    // Render header
    render_header();

    // Render based on state
    render_state(game);

    // Refresh screen
    refresh();
}

// Main game loop
int main(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGWINCH, signal_handler);

    g_game = malloc(sizeof(game_t));
    if (!g_game) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    game_init(g_game, "Cursed Tetris", "Player");

    // Initialize ncurses
    if (start_color(NULL) != OK) {
        fprintf(stderr, "Failed to initialize colors\n");
        free(g_game);
        return 1;
    }

    init_ncurses_colors();

    // Initialize ncurses
    if (start_color(NULL) != OK) {
        fprintf(stderr, "Failed to initialize ncurses\n");
        free(g_game);
        return 1;
    }

    if (init_curses() != OK) {
        fprintf(stderr, "Failed to initialize curses\n");
        free(g_game);
        return 1;
    }

    // Set terminal settings
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
    scrolloxoff(0, 0);
    curs_set(0);

    // Check terminal size
    int height, width;
    getmaxyx(stdscr, height, width);

    if (height < MIN_TERMINAL_HEIGHT || width < MIN_TERMINAL_WIDTH) {
        mvprintw(MIN_TERMINAL_HEIGHT - 1, 0, "Terminal too small. Minimum: %dx%d\n",
                 MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
        napms(100);
        endwin();
        free(g_game);
        return 1;
    }

    // Start game
    game_start(g_game);

    // Main loop
    while (g_game->running) {
        game_render_all(g_game);
        refresh();
        napms(16);  // ~60 FPS

        input_wait();
        game_update(g_game);
        game_handle_input(g_game);

        if (g_game->state == STATE_GAMEOVER) {
            game_render_all(g_game);
            refresh();
            napms(200);  // Slower after game over
        }
    }

    game_cleanup(g_game);
    free(g_game);

    return 0;
}
