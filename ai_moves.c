#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "core_gameplay.h"

void auto_place_ship(Board* board, int ship_length) {
  int x, y;
  char x_char, orientation;
  int res = -1;
  do {
    x = (rand() % BOARD_SIZE);
    y = (rand() % BOARD_SIZE) + 1;
    orientation = (rand() % 2 == 0) ? 'H' : 'V';

    res = place_ship(board, ship_length, NULL, 0, x, y, orientation, 0);
  } while (res < 0);
}

int auto_fire(Player* player, Player* enemy, int (*check_winner)()) {
  int x, y;
  char x_char;
  int res = -1;
  char* position = malloc(10 * sizeof(char));

  Player* players[2] = { player, enemy };
  do {
    x = (rand() % BOARD_SIZE);
    y = (rand() % BOARD_SIZE) + 1;

    res = place_bomb(player, enemy, NULL, 0, x, y, 0);

    int ress = check_winner(players);
    if (ress != -1) break;
  } while (res < 0);
  free(position);

  return res;
}
