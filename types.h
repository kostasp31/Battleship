#ifndef TYPES_H
#define TYPES_H

#define BOARD_SIZE 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define CLEAN_TERMINAL     "\e[1;1H\e[2J"

typedef enum { SEA, SHIP } Cell_Type;

typedef struct {
  Cell_Type type;
  int bombed;
} Cell;

typedef struct {
  Cell **board;
} Board;

typedef struct {
  char* name;
  int available_ships[5];
  int used_ships[5];
  Board* board;
} Player;

#endif