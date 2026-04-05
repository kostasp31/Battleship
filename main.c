#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "types.h"
#include "core_gameplay.h"
#include "manual_moves.h"
#include "ai_moves.h"
#include "print_ui.h"
#include "utils.h"

#define DELAY 500  // ms in printing delay

// TODO: smarter AI
// TODO: use arrows to place ships, bombs
int main() {
  srand(time(NULL));

  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Computer1", player1_board);
  Player* player2 = initialize_player("Computer2", player2_board);
  Player* players[2] = { player1, player2 };

  system("clear");
  printf(ANSI_COLOR_GREEN "=================== Game Begins! ====================\n" ANSI_COLOR_RESET);
  sleep_ms(DELAY);
  system("clear");
  printf(ANSI_COLOR_GREEN "================= Place your ships! =================\n" ANSI_COLOR_RESET);
  sleep_ms(DELAY);

  int ships[] = { 5, 4, 3, 3, 2 };
  for (int i=0; i<2; i++) {
    system("clear");
    for (int j=0; j<5; j++) {
      if (strncmp(players[i]->name, "Computer", 8) == 0) {
        system("clear");
        printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " places their ships...\n\n", players[i]->name);
        auto_place_ship(players[i]->board, ships[j]);
      } else {
        system("clear");
        printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " places their ships.\n\n", players[i]->name);
        print_board(players[i]->board);
        manual_place_ship(players[i]->board, ships[j]);
      }
    }
  }

  printf(ANSI_COLOR_GREEN "=================== Ships placed! ==================\n\n" ANSI_COLOR_RESET);
  sleep_ms(DELAY);
  system("clear");
  printf(ANSI_COLOR_GREEN "================= The game begins! =================\n\n" ANSI_COLOR_RESET);
  sleep_ms(DELAY);
  
  int turn = rand() % 2;
  while (1) {
    system("clear");
    printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " plays.\n\n", players[turn]->name);
    
    int res;
    int winner;
    if (strncmp(players[turn]->name, "Computer", 8) == 0) {
      res = auto_fire(players[turn], players[1 - turn], check_winner);
    } else {
      print_both_boards_for_player(players[turn], players[1 - turn]);
      printf("=> Position: ");
      res = manual_fire(players[turn], players[1 - turn], check_winner);
    }
    
    if (strncmp(players[turn]->name, "Computer", 8) != 0)
      system("clear");
    if (strncmp(players[turn]->name, "Computer", 8) != 0) {
      printf("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " played.\n\n", players[turn]->name);
      print_both_boards_for_player(players[turn], players[1 - turn]);
    }

    winner = check_winner(players) - 1;
    if (winner >= 0) {
      system("clear");
      printf("================= " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " WINS!!!! =================\n", players[1 - winner]->name);
      break;
    }
    
    if (res == 1) turn = turn;
    else turn = 1 - turn;

    sleep_ms((int) (DELAY + 0.5 * DELAY));
  }

  sleep_ms(DELAY);
  system("clear");
  printf(ANSI_COLOR_GREEN "================= The game ends! =================\n" ANSI_COLOR_RESET);
  sleep_ms(DELAY);
  system("clear");

  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);
}