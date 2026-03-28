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

int place_ship(Board* board, int ship_len, char* position, char orientation) {
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
    printf("ERROR [Ship out of bounds]\n");
    return -1;
  }

  if (orientation == 'H') {
    for (int i=col_int; i<col_int + ship_len; i++) {
      if (i >= BOARD_SIZE) {
        printf("ERROR [Ship out of bounds]\n");
        return -1;  
      }
      if (board->board[row_int][i].type == SHIP) {
        printf("ERROR [Ships collide]\n");
        return -2;     
      }
    }
    for (int i=col_int; i<col_int + ship_len; i++) {
      board->board[row_int][i].type = SHIP;
    }
  } else if (orientation == 'V') {
    for (int i=row_int; i<row_int + ship_len; i++) {
      if (i >= BOARD_SIZE) {
        printf("ERROR [Ship out of bounds]\n");
        return -1;  
      }
      if (board->board[i][col_int].type == SHIP) {
        printf("ERROR [Ships collide]\n");
        return -2;     
      }
    }
    for (int i=row_int; i<row_int + ship_len; i++) {
      board->board[i][col_int].type = SHIP;
    }
  }

  return 0;
}

int place_bomb(Player* player, Player* enemy, char* position) {
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
    printf("ERROR [Bomb falls out of bounds]\n");
    return -1;
  }

  Cell *target = &enemy->board->board[row_int][col_int];
  if (target->bombed) {
    printf("ERROR [You\'ve already bombed this position]\n");
    return -2;  
  }

  int hit = target->type == SHIP;
  (hit) ? printf("HIT!\n") : printf("MISSED\n");
  target->bombed = 1;

  return hit ? 1 : 0;
}

int check_winner(Player** players) {
  for (int player = 0; player<2; player++) {
    int won = 1;
    for (int i=0; i<BOARD_SIZE; i++) {
      if (!won) break;
      for (int j=0; j<BOARD_SIZE; j++) {
        if (players[player]->board->board[i][j].type == SHIP && !players[player]->board->board[i][j].bombed)
          won = 0;
      }
    }
    if (won) return player + 1;
  }
  return 0;
}

int main() {
  srand(time(NULL));

  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Kostas", player1_board);
  Player* player2 = initialize_player("Takis", player2_board);
  Player* players[2] = { player1, player2 };

  printf("Place your ships!\n");
  for (int i=0; i<2; i++) {
    printf("%s places their ships.\n", players[i]->name);
    int ships[] = { 5, 4, 3, 3, 2 };
    for (int j=0; j<1; j++) { // j < 5
      
      char* pos = malloc(3 * sizeof(char));
      char orient;
      
      int res;
      do {
        printf("Ship length: %d\n", ships[j]);
        scanf("%s %c[^\n]", pos, &orient);
        fflush(stdin);
        res = place_ship(players[i]->board, ships[j], pos, orient);
      } while (res < 0);

      free(pos);
      print_board(players[i]->board);
    }
  }

  int turn = rand() % 2;
  while (1) {
    printf("%s plays.\n", players[turn]->name);
    char* pos = malloc(3 * sizeof(char));
    int res;
    do {
      scanf("%s[^\n]", pos);
      fflush(stdin);
      res = place_bomb(players[turn], players[1-turn], pos);
      printf("%d", res);
    } while (res < 0);
    free(pos);
    
    print_board(players[0]->board);
    print_board(players[1]->board);

    if (res == 1) turn = turn;
    else turn = 1 - turn;

    int winner = check_winner(players);
    if (winner) {
      printf("================= %s WINS!!!! =================\n", players[winner-1]->name);
      break;
    }
  }

  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);
}