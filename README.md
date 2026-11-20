# Cursed Tetris

A standard game of Tetris for the terminal, written in C with ncurses.
Pieces are rendered as colored `[]` blocks; the board is 10x20 with a
next-piece preview, score/lines/level panel, pause and restart.

## Requirements

- C compiler (gcc or clang)
- CMake >= 3.10
- ncurses development headers
  - Debian/Ubuntu: `sudo apt install libncurses-dev`
  - Fedora: `sudo dnf install ncurses-devel`
  - macOS: included in the system SDK

## Build & run

```sh
cmake -S . -B build
cmake --build build
./build/cursed-tetris
```

A terminal of at least 62x24 is required.

## Controls

| Key          | Action            |
| ------------ | ----------------- |
| h / l (arrows) | move left/right |
| j / up       | rotate clockwise  |
| k            | rotate counter-clockwise |
| m / down     | soft drop         |
| space        | hard drop         |
| p            | pause / resume    |
| r            | restart (on game over) |
| q            | quit              |

## License

MIT - see [LICENSE](LICENSE).
