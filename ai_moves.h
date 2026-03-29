#ifndef AI_H
#define AI_H

#include "types.h"

void auto_place_ship(Board*, int);
int auto_fire(Player*, Player*, int (*)());

#endif