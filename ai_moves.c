#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "core_gameplay.h"

void auto_place_ship(Board* board, int ship_length) {
  int x, y;
  char x_char, orientation;
  int res = -1;
  char* position = malloc(10 * sizeof(char));
  do {
    x = (rand() % BOARD_SIZE);
    x_char = x + 65;
    y = (rand() % BOARD_SIZE) + 1;
    orientation = (rand() % 2 == 0) ? 'H' : 'V';
    sprintf(position, "%c%d %c", x_char, y, orientation);

    res = place_ship(board, ship_length, position, orientation, 0);
  } while (res < 0);
  free(position);
}

int auto_fire(Player* player, Player* enemy, int (*check_winner)()) {
  int x, y;
  char x_char;
  int res = -1;
  char* position = malloc(10 * sizeof(char));

  Player* players[2] = { player, enemy };
  do {
    x = (rand() % BOARD_SIZE);
    x_char = x + 65;
    y = (rand() % BOARD_SIZE) + 1;
    sprintf(position, "%c%d", x_char, y);

    res = place_bomb(player, enemy, position, 0);

    int ress = check_winner(players);
    if (ress != -1) break;
  } while (res < 0);
  free(position);

  return res;
}
