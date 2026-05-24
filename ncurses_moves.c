#include "ncurses.h"
#include "types.h"
#include "print_ui.h"
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
          if (abs_x >= BOARD_SIZE || abs_y >= BOARD_SIZE || abs_x < 0 || abs_y == 0) continue;

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