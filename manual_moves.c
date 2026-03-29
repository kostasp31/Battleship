#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "core_gameplay.h"

int manual_fire(Player* player, Player* enemy, int (*check_winner)()) {
  int res;
  char* pos = malloc(3 * sizeof(char));
  do {
    scanf("%s[^\n]", pos);
    fflush(stdin);
    res = place_bomb(player, enemy, pos, 1);
  } while (res < 0);
  free(pos);
  return res;
}

void manual_place_ship(Board* board, int ship_length) {
  char* pos = malloc(3 * sizeof(char));
  char orient;
  
  int res;
  do {
    printf("Ship length: " ANSI_COLOR_CYAN "%d" ANSI_COLOR_RESET ". Position => ", ship_length);
    scanf("%s %c[^\n]", pos, &orient);
    fflush(stdin);
    res = place_ship(board, ship_length, pos, orient, 1);
  } while (res < 0);
  
  free(pos);
}
