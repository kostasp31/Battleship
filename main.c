#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "types.h"
#include "core_gameplay.h"
#include "manual_moves.h"
#include "ai_moves.h"
#include "print_ui.h"

// TODO: smarter AI
int main() {
  srand(time(NULL));

  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Computer1", player1_board);
  Player* player2 = initialize_player("Computer2", player2_board);
  Player* players[2] = { player1, player2 };

  printf(ANSI_COLOR_GREEN "================= Place your ships! =================\n" ANSI_COLOR_RESET);
  int ships[] = { 5, 4, 3, 3, 2 };
  for (int i=0; i<2; i++) {
    printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " places their ships.\n", players[i]->name);
    for (int j=0; j<5; j++) {
      if (strncmp(players[i]->name, "Computer", 8) == 0) {
        auto_place_ship(players[i]->board, ships[j]);
      } else {
        manual_place_ship(players[i]->board, ships[j]);
        print_board(players[i]->board);
      }
    }
  }
  
  int turn = rand() % 2;
  printf(ANSI_COLOR_GREEN "================= The game begins! =================\n" ANSI_COLOR_RESET);
  while (1) {
    printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " plays. ", players[turn]->name);
    (strncmp(players[turn]->name, "Computer", 8) == 0) ? printf("\n") : printf("Position: ");
    
    int res;
    int winner;
    if (strncmp(players[turn]->name, "Computer", 8) == 0) {
      res = auto_fire(players[turn], players[1 - turn], check_winner);
    } else {
      res = manual_fire(players[turn], players[1 - turn], check_winner);
    }
    
    if (strncmp(players[turn]->name, "Computer", 8) != 0)
      print_both_boards_for_player(players[turn], players[1 - turn]);

    winner = check_winner(players) - 1;
    if (winner >= 0) {
      printf("================= " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " WINS!!!! =================\n", players[1 - winner]->name);
      break;
    }
    
    if (res == 1) turn = turn;
    else turn = 1 - turn;
  }
  printf(ANSI_COLOR_GREEN "================= The game ends! =================\n" ANSI_COLOR_RESET);

  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);
}