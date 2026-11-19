cmake_minimum_required(VERSION 3.10)
project(cursed-tetris VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Get the SDK path for ncurses include files
execute_process(
    COMMAND "${CMAKE_COMMAND}" -E env "PATH=$ENV{PATH}" xcrun --show-sdk-path
    OUTPUT_VARIABLE SDK_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(NCURSES_INCLUDE_DIR "${SDK_PATH}/usr/include")

# Find ncurses
find_path(NCURSES_INCLUDE_DIRS
    NAMES ncurses.h
    PATHS "${NCURSES_INCLUDE_DIR}"
    NO_DEFAULT_PATH
)

if(NOT NCURSES_INCLUDE_DIRS)
    message(FATAL_ERROR "ncurses not found at ${NCURSES_INCLUDE_DIR}")
endif()

# Find required libraries
find_library(NCURSES_LIBRARY ncurses REQUIRED)

# On Windows, we also need windows library for color support
if(WIN32)
    find_library(WINDOWS_LIBRARY windows REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${NCURSES_LIBRARY} ${WINDOWS_LIBRARY})
else()
    target_link_libraries(${PROJECT_NAME} ${NCURSES_LIBRARY})
endif()

# Set include directories
include_directories(${NCURSES_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable
add_executable(${PROJECT_NAME}
    main.c
    src/tetromino.c
    src/board.c
    src/game.c
    src/input.c
)

# Compilation flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
