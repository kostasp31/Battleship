#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

void print_board(Board* board) {
  printf("   ");
  for (int j=0; j<BOARD_SIZE; j++) {
    (j == BOARD_SIZE - 1) ? printf("  %c  \n", j + 65) : printf("  %c ", j + 65);
  }
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? printf("+---+\n") : printf("+---");
    }

    printf("%2d ", i+1);
    for (int j=0; j<BOARD_SIZE; j++) {
      printf("| ");

      Cell_Type type = board->board[i][j].type;
      int bombed = board->board[i][j].bombed;
  
      if (type == SEA) {
        bombed ? printf(ANSI_COLOR_YELLOW) : printf(ANSI_COLOR_CYAN);
        bombed ? printf("*") : printf(" ");
      } else {
        bombed ? printf(ANSI_COLOR_RED) : printf(ANSI_COLOR_GREEN);
        printf("V");
      }
      printf(ANSI_COLOR_RESET);

      (j == BOARD_SIZE - 1) ? printf(" |\n") : printf(" ");
    }
  }
  for (int j=0; j<BOARD_SIZE; j++) {
    (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? printf("+---+\n\n") : printf("+---");
  }
}

void print_both_boards_debug(Player* player1, Player* player2) {
  printf("  \t\t%s fleet  \t\t\t\t\t%s fleet\n", player1->name, player2->name);
  for (int b=0; b<2; b++) {
    printf("   ");
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == BOARD_SIZE - 1) ? (b == 0) ? printf("  %c  \t", j + 65) :  printf("  %c  \n", j + 65) : printf("  %c ", j + 65);
    }
  }
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int b=0; b<2; b++) {
      for (int j=0; j<BOARD_SIZE; j++) {
        (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? (b == 0) ? printf("+---+\t") : printf("+---+\n") : printf("+---");
      }
    }

    for (int b=0; b<2; b++) {
      printf("%2d ", i+1);
      for (int j=0; j<BOARD_SIZE; j++) {
        printf("| ");

        Cell_Type type = (b == 0) ? player1->board->board[i][j].type : player2->board->board[i][j].type;
        int bombed = (b == 0) ? player1->board->board[i][j].bombed : player2->board->board[i][j].bombed;
    
        if (type == SEA) {
          bombed ? printf(ANSI_COLOR_YELLOW) : printf(ANSI_COLOR_CYAN);
          bombed ? printf("*") : printf(" ");
        } else {
          bombed ? printf(ANSI_COLOR_RED) : printf(ANSI_COLOR_GREEN);
          printf("V");
        }
        printf(ANSI_COLOR_RESET);

        (j == BOARD_SIZE - 1) ? (b == 0) ? printf(" |\t") : printf(" |\n") : printf(" ");
      }
    }
  }

  for (int b=0; b<2; b++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? (b == 0) ? printf("+---+\t") : printf("+---+\n\n") : printf("+---");
    }
  }
}

void print_both_boards_for_player(Player* player1, Player* player2) {
  printf("  \t\t%s fleet  \t\t\t\t\t%s fleet\n", player1->name, player2->name);
  for (int b=0; b<2; b++) {
    printf("   ");
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == BOARD_SIZE - 1) ? (b == 0) ? printf("  %c  \t", j + 65) :  printf("  %c  \n", j + 65) : printf("  %c ", j + 65);
    }
  }
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int b=0; b<2; b++) {
      for (int j=0; j<BOARD_SIZE; j++) {
        (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? (b == 0) ? printf("+---+\t") : printf("+---+\n") : printf("+---");
      }
    }

    for (int b=0; b<2; b++) {
      printf("%2d ", i+1);
      for (int j=0; j<BOARD_SIZE; j++) {
        printf("| ");

        Cell_Type type = (b == 0) ? player1->board->board[i][j].type : player2->board->board[i][j].type;
        int bombed = (b == 0) ? player1->board->board[i][j].bombed : player2->board->board[i][j].bombed;
    
        if (type == SEA) {
          bombed ? printf(ANSI_COLOR_YELLOW) : printf(ANSI_COLOR_CYAN);
          bombed ? printf("*") : printf(" ");
        } else {
          bombed ? printf(ANSI_COLOR_RED) : printf(ANSI_COLOR_GREEN);
          (b == 0) ? printf("V") : bombed ? printf("*") : printf(" ");
        }
        printf(ANSI_COLOR_RESET);

        (j == BOARD_SIZE - 1) ? (b == 0) ? printf(" |\t") : printf(" |\n") : printf(" ");
      }
    }
  }

  for (int b=0; b<2; b++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == 0) ? printf("   +---") : (j == BOARD_SIZE - 1) ? (b == 0) ? printf("+---+\t") : printf("+---+\n\n") : printf("+---");
    }
  }
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
  (hit) ? printf("HIT!\n") : printf("MISSED\n");
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

    res = place_ship(board, ship_length, position, orientation, 1); // TODO: put 0
  } while (res < 0);
  free(position);
}

void manual_place_ship(Board* board, int ship_length) {
  char* pos = malloc(3 * sizeof(char));
  char orient;
  
  int res;
  do {
    printf("Ship length: %d\n", ship_length);
    scanf("%s %c[^\n]", pos, &orient);
    fflush(stdin);
    res = place_ship(board, ship_length, pos, orient, 1);
  } while (res < 0);
  
  free(pos);
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

int manual_fire(Player* player, Player* enemy, int (*check_winner)()) {
  int res;
  char* pos = malloc(3 * sizeof(char));
  do {
    scanf("%s[^\n]", pos);
    fflush(stdin);
    res = place_bomb(player, enemy, pos, 1);
    printf("%d", res);
  } while (res < 0);
  free(pos);
  return res;
}

// TODO: smarter AI
int main() {
  srand(time(NULL));

  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Computer1", player1_board);
  Player* player2 = initialize_player("Computer2", player2_board);
  Player* players[2] = { player1, player2 };


  printf("Place your ships!\n");
  int ships[] = { 5, 4, 3, 3, 2 };
  for (int i=0; i<2; i++) {
    printf("%s places their ships.\n", players[i]->name);
    for (int j=0; j<5; j++) { // j < 5
      if (strncmp(players[i]->name, "Computer", 8) == 0) {
        auto_place_ship(players[i]->board, ships[j]);
      } else {
        manual_place_ship(players[i]->board, ships[j]);
      }
    }
  }

  int turn = rand() % 2;
  while (1) {
    printf("%s plays.\n", players[turn]->name);

    int res;
    int winner;
    if (strncmp(players[turn]->name, "Computer", 8) == 0) {
      res = auto_fire(players[turn], players[1 - turn], check_winner);
    } else {
      res = manual_fire(players[turn], players[1 - turn], check_winner);
    }
    
    print_both_boards_for_player(players[turn], players[1 - turn]);
    winner = check_winner(players) - 1;
    if (winner >= 0) {
      printf("================= %s WINS!!!! =================\n", players[1 - winner]->name);
      break;
    }
    
    if (res == 1) turn = turn;
    else turn = 1 - turn;
  }

  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);
}