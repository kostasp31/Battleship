#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <ncurses.h>
#include "types.h"
#include "utils.h"

//////////////////// STANDARD PRINTING FUNCTIONS ////////////////////

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

//////////////////// NCURSES PRINTING FUNCTIONS ////////////////////

// print board centered on window x and y
// return array of [y, x] coordinates
void print_board_ncurses(WINDOW* win, int board_win_height, int board_win_width, Board* board) {
  int board_height = (2 * BOARD_SIZE) + 1 + 1; // 2*board_size:  
  int board_width = (4 * BOARD_SIZE) + ((BOARD_SIZE / 10) + 1) + 1;
  int board_y_cursor = (int) ((board_win_height / 2) - (board_height / 2));
  int board_x_cursor = (int) ((board_win_width / 2) - (board_width / 2));

  wmove(win, board_y_cursor, board_x_cursor);
  wprintw(win, "   ");
  for (int j=0; j<BOARD_SIZE; j++) {
    wprintw(win, "  %c ", j + 65);
    if (j == BOARD_SIZE - 1) wmove(win, ++board_y_cursor, board_x_cursor);
  }
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      (j == 0) ? wprintw(win, "   +---") : (j == BOARD_SIZE - 1) ? wprintw(win, "+---+") : wprintw(win, "+---");
      if (j == BOARD_SIZE - 1) wmove(win, ++board_y_cursor, board_x_cursor);
    }

    wprintw(win, "%2d ", i+1);
    for (int j=0; j<BOARD_SIZE; j++) {
      wprintw(win, "| ");

      Cell_Type type = board->board[i][j].type;
      int bombed = board->board[i][j].bombed;
  
      if (type == SEA) {
        bombed ? wprintw(win, "*") : wprintw(win, " ");
      } else {
        wprintw(win, "V");
      }

      (j == BOARD_SIZE - 1) ? wprintw(win, " |") : wprintw(win, " ");
      if (j == BOARD_SIZE - 1) wmove(win, ++board_y_cursor, board_x_cursor);
    }
  }
  for (int j=0; j<BOARD_SIZE; j++) {
    (j == 0) ? wprintw(win, "   +---") : (j == BOARD_SIZE - 1) ? wprintw(win, "+---+") : wprintw(win, "+---");
    if (j == BOARD_SIZE - 1) wmove(win, ++board_y_cursor, board_x_cursor);
  }

  wrefresh(win);
  return;
}