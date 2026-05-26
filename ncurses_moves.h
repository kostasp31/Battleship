#ifndef NCURSES_MOVES_H
#define NCURSES_MOVES_H

#include "types.h"
#include "ncurses.h"

void ncurses_place_ships(WINDOW*, int [], int, int, int, Board*, char*, int, MEVENT*);
int ncurses_play_game(WINDOW*, MEVENT*, Player* [2], int, int, int, int, int);

#endif