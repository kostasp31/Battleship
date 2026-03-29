#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

Player* initialize_player(char* name, Board* board) {
  Player* player = malloc(sizeof(Player));

  int name_len = strlen(name);
  player->name = malloc(name_len * sizeof(char));
  strncpy(player->name, name, name_len);
  player->board = board;
}

void destroy_player(Player* player) {
  free(player->name);
  free(player);
}

Board* initialize_board() {
  Board* board = malloc(sizeof(Board));
  board->board = malloc(BOARD_SIZE * sizeof(Cell*));
  for (int i=0; i<BOARD_SIZE; i++) {
    board->board[i] = malloc(BOARD_SIZE * sizeof(Cell));
  }
  
  
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      Cell cell;
      cell.type = SEA;
      cell.bombed = 0; 
      board->board[i][j] = cell;
    }
  }

  return board;
}

void destroy_board(Board* board) {
  for (int i=0; i<BOARD_SIZE; i++)
    free(board->board[i]);
  free(board->board);
  free(board);
}


int place_ship(Board* board, int ship_len, char* position, char orientation, int log) {
  // parse position x and y
  char col_char = position[0];
  int col_int = col_char - 65;
  
  int pos_size = strlen(position);
  char* row_str = malloc((pos_size - 1) * sizeof(char));
  for (int i=0; i<pos_size - 1; i++) {
    row_str[i] = position[i+1];
  }
  int row_int = atoi(row_str) - 1;
  free(row_str);
  
  if (row_int >= BOARD_SIZE || col_int >= BOARD_SIZE) {
    if (log) printf(ANSI_COLOR_RED "ERROR [Ship out of bounds]\n" ANSI_COLOR_RESET);
    return -1;
  }

  if (orientation == 'H') {
    for (int i=col_int; i<col_int + ship_len; i++) {
      if (i >= BOARD_SIZE) {
        if (log) printf(ANSI_COLOR_RED "ERROR [Ship out of bounds]\n" ANSI_COLOR_RESET);
        return -1;  
      }
      if (board->board[row_int][i].type == SHIP) {
        if (log) printf(ANSI_COLOR_RED "ERROR [Ships collide]\n" ANSI_COLOR_RESET);
        return -2;     
      }
    }
    for (int i=col_int; i<col_int + ship_len; i++) {
      board->board[row_int][i].type = SHIP;
    }
  } else if (orientation == 'V') {
    for (int i=row_int; i<row_int + ship_len; i++) {
      if (i >= BOARD_SIZE) {
        if (log) printf(ANSI_COLOR_RED "ERROR [Ship out of bounds]\n" ANSI_COLOR_RESET);
        return -1;  
      }
      if (board->board[i][col_int].type == SHIP) {
        if (log) printf(ANSI_COLOR_RED "ERROR [Ships collide]\n" ANSI_COLOR_RESET);
        return -2;     
      }
    }
    for (int i=row_int; i<row_int + ship_len; i++) {
      board->board[i][col_int].type = SHIP;
    }
  }

  return 0;
}

int place_bomb(Player* player, Player* enemy, char* position, int log) {
  // parse position x and y
  char col_char = position[0];
  int col_int = col_char - 65;
  
  int pos_size = strlen(position);
  char* row_str = malloc((pos_size - 1) * sizeof(char));
  for (int i=0; i<pos_size - 1; i++) {
    row_str[i] = position[i+1];
  }
  int row_int = atoi(row_str) - 1;
  free(row_str);
  
  if (row_int >= BOARD_SIZE || col_int >= BOARD_SIZE) {
    if (log) printf(ANSI_COLOR_RED "ERROR [Bomb falls out of bounds: %d, %d]\n" ANSI_COLOR_RESET, row_int, col_int);
    return -1;
  }

  Cell *target = &enemy->board->board[row_int][col_int];
  if (target->bombed) {
    if (log) printf(ANSI_COLOR_RED "ERROR [You\'ve already bombed this position: %d, %d]\n" ANSI_COLOR_RESET, row_int, col_int);
    return -2;  
  }

  int hit = target->type == SHIP;
  // (hit) ? printf("HIT!\n") : printf("MISSED\n");
  (hit) ? printf(ANSI_COLOR_CYAN "\t=> * HIT!\n" ANSI_COLOR_RESET) : printf(ANSI_COLOR_RED "\t=> X MISSED!\n" ANSI_COLOR_RESET);
  target->bombed = 1;

  return hit ? 1 : 0;
}

int check_winner(Player** players) {
  for (int player = 0; player<2; player++) {
    int lost = 1;
    for (int i=0; i<BOARD_SIZE; i++) {
      if (!lost) break;
      for (int j=0; j<BOARD_SIZE; j++) {
        if (players[player]->board->board[i][j].type == SHIP && !players[player]->board->board[i][j].bombed) {
          lost = 0;
          break;
        }
      }
    }
    if (lost) return player + 1;
  }
  return -1;
}
