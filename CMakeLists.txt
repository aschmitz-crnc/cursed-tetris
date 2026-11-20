cmake_minimum_required(VERSION 3.10)
project(cursed-tetris VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Help IDEs (clangd etc.) resolve includes
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ncurses (portable: handles macOS SDK / system prefixes for us)
find_package(Curses REQUIRED)

# POSIX.1-2008 for clock_gettime()/CLOCK_MONOTONIC under -std=c99
add_definitions(-D_POSIX_C_SOURCE=200809L)

add_executable(${PROJECT_NAME}
    main.c
    src/tetromino.c
    src/board.c
    src/game.c
    src/input.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
