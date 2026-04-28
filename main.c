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

#include "ascii_art.h"

#include <ncurses.h>
#include <panel.h>

#define DELAY 500  // ms in printing delay

// SEE: https://github.com/mcdaniel/curses_tutorial
// FOR ART: https://ascii.co.uk/art/battleship

// TODO: smarter AI
// TODO: use arrows to place ships, bombs (ncurses)
int main() {
  initscr();
  cbreak(); // Disable line buffering (read input immediately)
  noecho(); // Don't echo input characters to the screen
  keypad(stdscr, TRUE); // Enable special keys (arrow keys, F1-F12, etc.)
  curs_set(1);  // 0 for invisible cursor, 1 visible
  raw();

  int screen_x, screen_y; // terminal dimensions
  MEVENT event;
  WINDOW *title_win, *log_win, *board_win;  // 2 windows: 1 for the instructions, 1 for the boards

  // mouse stuff
  mmask_t mask, oldmask;
  mask = BUTTON1_CLICKED;
  mousemask(mask, &oldmask); // Assigns new mask and saves previous one
  mouseinterval(0); // No delay for mouse clicks

  getmaxyx(stdscr, screen_y, screen_x); // get terminal dimensions
  title_win = newwin(screen_y, screen_x, 0, 0);
  
  srand(time(NULL));
  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Kostas", player1_board);
  Player* player2 = initialize_player("Computer", player2_board);
  Player* players[2] = { player1, player2 };

  
  extern const char *ship_art[];
  extern const char *title_art[];
  extern const char *start_button_art[];
  extern const char *starting1_message_art[];
  extern const char *starting2_message_art[];
  extern const char *starting3_message_art[];
  extern const char* place_your_fleet_art[];
  extern const int ship_art_dim[];
  extern const int title_art_dim[];
  extern const int start_button_art_dim[];
  extern const int starting1_message_art_dim[];
  extern const int starting2_message_art_dim[];
  extern const int starting3_message_art_dim[];
  extern const int place_your_fleet_art_dim[];

  // print warship
  int starty = (int) (0.1 * screen_y);
  int startx = (int) (screen_x / 2) - (ship_art_dim[0] / 2);
  for (int i = 0; i < ship_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", ship_art[i]);
  }
  
  // print title
  starty += 13;
  startx = (int) (screen_x / 2) - (title_art_dim[0] / 2);
  for (int i = 0; i < title_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", title_art[i]);
  }

  // print start button
  starty = (int) screen_y - (0.15 * screen_y);
  startx = (int) (screen_x / 2) - (start_button_art_dim[0] / 2);
  for (int i = 0; i < start_button_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", start_button_art[i]);
  }
  wrefresh(title_win);

  int ch;
  int in_title_screen = 1;
  while (in_title_screen) {
    ch = getch();
    if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_CLICKED) {
          if (event.x >= startx && event.x <= startx + start_button_art_dim[0]) {
            if (event.y >= starty && event.y <= starty + start_button_art_dim[1]) {
              wrefresh(title_win);
              in_title_screen = 0;
              break;
            }
          }
        }
      }
    }
  }

  wclear(title_win);

  startx = (int) (screen_x / 2) - (starting1_message_art_dim[0] / 2);
  starty = (int) (screen_y / 2) - (starting1_message_art_dim[1] / 2);
  for (int i = 0; i < starting1_message_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", starting1_message_art[i]);
  }
  wrefresh(title_win);
  refresh();
  sleep_ms(350);

  for (int i = 0; i < starting1_message_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", starting2_message_art[i]);
  }
  wrefresh(title_win);
  refresh();
  sleep_ms(350);

  for (int i = 0; i < starting1_message_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", starting3_message_art[i]);
  }
  wrefresh(title_win);
  refresh();
  sleep_ms(350);

  startx = (int) (screen_x / 2) - (place_your_fleet_art_dim[0] / 2);
  starty = (int) (screen_y / 2) - (place_your_fleet_art_dim[1] / 2);
  for (int i = 0; i < starting1_message_art_dim[1]; i++) {
    mvprintw(starty + i, startx, "%s", place_your_fleet_art[i]);
  }
  wrefresh(title_win);

  getch();

  wclear(title_win);
  wrefresh(title_win);
  wdelch(title_win);

  // delete title window and create new regions 
  // +--------+---------------+
  // |  LOGS  |     BOARDS    |
  // +--------+---------------+
  log_win = newwin(20, 20, 0, 0);
  board_win = newwin(screen_y,  (int) screen_x * 0.66, 0, (int) screen_x * 0.33 + 1);

  box(log_win, 20, 20);
  wrefresh(log_win);
  refresh();

  getch();

  // int ships[] = { 5, 4, 3, 3, 2 };
  // for (int i=0; i<2; i++) {
  //   clear();
  //   for (int j=0; j<5; j++) {
  //     if (strncmp(players[i]->name, "Computer", 8) == 0) {
  //       clear();
  //       printw("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " places their ships...\n\n", players[i]->name);
  //       auto_place_ship(players[i]->board, ships[j]);
  //     } else {
  //       clear();
  //       printw("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " places their ships.\n\n", players[i]->name);
  //       print_board(players[i]->board);
  //       manual_place_ship(players[i]->board, ships[j]);
  //     }
  //   }
  // }

  // printw(ANSI_COLOR_GREEN "=================== Ships placed! ==================\n\n" ANSI_COLOR_RESET);
  // sleep_ms(DELAY);
  // clear();
  // printw(ANSI_COLOR_GREEN "================= The game begins! =================\n\n" ANSI_COLOR_RESET);
  // sleep_ms(DELAY);
  
  // int turn = rand() % 2;
  // while (1) {
  //   clear();
  //   printw("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " plays.\n\n", players[turn]->name);
    
  //   int res;
  //   int winner;
  //   if (strncmp(players[turn]->name, "Computer", 8) == 0) {
  //     res = auto_fire(players[turn], players[1 - turn], check_winner);
  //   } else {
  //     print_both_boards_for_player(players[turn], players[1 - turn]);
  //     printw("=> Position: ");
  //     res = manual_fire(players[turn], players[1 - turn], check_winner);
  //   }
    
  //   if (strncmp(players[turn]->name, "Computer", 8) != 0)
  //     clear();
  //   if (strncmp(players[turn]->name, "Computer", 8) != 0) {
  //     printw("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " played.\n\n", players[turn]->name);
  //     print_both_boards_for_player(players[turn], players[1 - turn]);
  //   }

  //   winner = check_winner(players) - 1;
  //   if (winner >= 0) {
  //     clear();
  //     printw("================= " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " WINS!!!! =================\n", players[1 - winner]->name);
  //     break;
  //   }
    
  //   if (res == 1) turn = turn;
  //   else turn = 1 - turn;

  //   sleep_ms((int) (DELAY + 0.5 * DELAY));
  // }

  // sleep_ms(DELAY);
  // clear();
  // printw(ANSI_COLOR_GREEN "================= The game ends! =================\n" ANSI_COLOR_RESET);
  // sleep_ms(DELAY);
  // clear();

  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);

  endwin();
  echo();
  curs_set(1);
}