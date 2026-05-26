#include "ncurses.h"
#include "types.h"
#include "print_ui.h"
#include "ai_moves.h"
#include "utils.h"
#include "core_gameplay.h"

// ship preview
void ncurses_print_ship_preview(WINDOW* win, int ship_len, char orientation) {
  // erase old
  for (int k=0; k<5; k++) {
    mvwprintw(win, k + 3, 1, " ");
    mvwprintw(win, 3, k + 1, " ");
  }
  // new preview
  for (int k=0; k<ship_len; k++) {
    if (orientation == 'H') mvwprintw(win, 3, k + 1, "V");
    else mvwprintw(win, k + 3, 1, "V");
  }

  wrefresh(win);
}

void ncurses_place_ships(WINDOW* win, int ships[], int j, int board_win_height, int board_win_width, Board* board, char* orientation, int log_win_width, MEVENT* event) {
  int top_left_corner_y = 0;
  int top_left_corner_x = 0;

  int ship_len = ships[j];
  print_board_ncurses(win, board_win_height, board_win_width, board, &top_left_corner_y, &top_left_corner_x);
  if (!j) mvwprintw(win, 1, 1, "Place your first ship   ");
  mvwprintw(win, 2, 1, "Orientation: %s", *orientation == 'H' ? "Horizontal" : "Vertical  ");

  ncurses_print_ship_preview(win, ship_len, *orientation);

  wrefresh(win);
  int ch = 0;
  while (ch = wgetch(win)) {
    if (ch == KEY_MOUSE) {
      if (getmouse(event) == OK) {
        // right click to change *orientation
        if (event->bstate & BUTTON3_CLICKED) {
          if (*orientation == 'H') *orientation = 'V';
          else *orientation = 'H';
          mvwprintw(win, 2, 1, "Orientation: %s", *orientation == 'H' ? "Horizontal" : "Vertical  ");

          ncurses_print_ship_preview(win, ship_len, *orientation);
          continue;
        }
        // left click to place ship if valid position
        else if (event->bstate & BUTTON1_CLICKED) {
          int norm_y = event->y - top_left_corner_y;
          int norm_x = event->x - log_win_width - top_left_corner_x;
          if (norm_y % 2 == 0 || norm_x % 4 == 0) continue;

          int abs_x = (int) norm_x / 4;
          int abs_y = (int) norm_y / 2;

          // out of board bounds, try again
          if (abs_x >= BOARD_SIZE || abs_y >= BOARD_SIZE || abs_x < 0 || abs_y < 0) continue;

          const int retVal = place_ship(board, ships[j], NULL, 0, abs_x, abs_y, *orientation, 0);
          if (retVal != 0) {
            mvwprintw(win, 1, 1, "Cant place a ship there");
            wrefresh(win);
            continue;
          }

          mvwprintw(win, 1, 1, "Placed ship at: (%d, %d)  ", abs_x, abs_y);
          if (j == 4) print_board_ncurses(win, board_win_height, board_win_width, board, &top_left_corner_y, &top_left_corner_x);
          wrefresh(win);
          break;
        }
      }
    }
  }
}

// main game loop, return 0 or 1 for index of winner, negative for failure
int ncurses_play_game(WINDOW* win, MEVENT* event, Player* players[2], int turn, int board_win_height, int board_win_width, int log_win_width, int delay_ms) {
  int top_left_corner_y_both = 0;
  int top_left_corner_x_both = 0;
  while (1) {
    int res;
    int winner;
    if (strncmp(players[turn]->name, "Computer", 8) == 0) {
      res = auto_fire(players[turn], players[1 - turn], check_winner);
      // print_both_boards_ncurses(win, board_win_height, board_win_width, players[0], players[1], &top_left_corner_y_both, &top_left_corner_x_both);
    } else {
      print_both_boards_ncurses(win, board_win_height, board_win_width, players[turn], players[1 - turn], &top_left_corner_y_both, &top_left_corner_x_both);
      int ch = 0;
      while (ch = wgetch(win)) {
        if (ch == KEY_MOUSE) {
          if (getmouse(event) == OK) {
            // left click to place bomb if valid position
            if (event->bstate & BUTTON1_CLICKED) {
              int norm_y = event->y - top_left_corner_y_both;
              int norm_x = event->x - log_win_width - top_left_corner_x_both;
              if (norm_y % 2 == 0 || norm_x % 4 == 0) continue;

              int abs_x = (int) norm_x / 4;
              int abs_y = (int) norm_y / 2;

              // out of board bounds, try again
              if (abs_x >= BOARD_SIZE || abs_y >= BOARD_SIZE || abs_x < 0 || abs_y < 0) continue;

              res = place_bomb(players[turn], players[1 - turn], NULL, 0, abs_x, abs_y, 0);
              if (res < 0) {
                mvwprintw(win, 1, 1, "Cant place a bomb there");
                wrefresh(win);
                continue;
              }

              mvwprintw(win, 1, 1, "Placed ship at: (%d, %d)  ", abs_x, abs_y);
              print_both_boards_ncurses(win, board_win_height, board_win_width, players[turn], players[1 - turn], &top_left_corner_y_both, &top_left_corner_x_both);
              wrefresh(win);
              break;
            }
          }
        }
      }
    }
    
    // if (strncmp(players[turn]->name, "Computer", 8) != 0)
    //   clear();
    // if (strncmp(players[turn]->name, "Computer", 8) != 0) {
    //   printw("=> " ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET " played.\n\n", players[turn]->name);
    //   print_both_boards_for_player(players[turn], players[1 - turn]);
    // }

    winner = check_winner(players) - 1;
    if (winner >= 0) return winner;
    
    if (res == 1) turn = turn;
    else turn = 1 - turn;

    if (delay_ms) sleep_ms(delay_ms);
  }

}