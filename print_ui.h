#ifndef PRINT_H
#define PRINT_H

#include "types.h"
#include <ncurses.h>

void print_board(Board*);
void print_both_boards_debug(Player*, Player*);
void print_both_boards_for_player(Player*, Player*);

void print_ascii_art(int, int, const int[], const char*[]);

#endif