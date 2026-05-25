#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

Player* initialize_player(char*, Board*);
void destroy_player(Player*);
Board* initialize_board();
void destroy_board(Board*);
int place_ship(Board*, int, char*, int, int, int, char, int);
int place_bomb(Player*, Player*, char*, int, int, int, int);
int check_winner(Player**);

#endif