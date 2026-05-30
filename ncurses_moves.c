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

    // TODO: fix bug here...
    winner = check_winner(players) - 1;
    if (winner >= 0) return winner;
    
    if (res == 1) turn = turn;
    else turn = 1 - turn;

    if (delay_ms) sleep_ms(delay_ms);
  }
}

int ncurses_winner_screen(int winner, int screen_x, int screen_y) {
  extern const char* you_win_art[];
  extern const char* computer_wins_art[];
  extern const int you_win_dim[];
  extern const int computer_wins_dim[];

  // print winner message
  if (winner == 0) {  // player won
    int startx = (int) (screen_x / 2) - (you_win_dim[0] / 2);
    int starty = (int) (screen_y / 2) - (you_win_dim[1] / 2);
    clear();
    for (int i = 0; i < you_win_dim[1]; i++) {
      mvprintw(starty + i, startx, "%s", you_win_art[i]);
    }

    const int non_editable_x_start = startx;
    const int non_editable_x_end = startx + you_win_dim[0] - 1;    
    const int non_editable_y_start = starty;
    const int non_editable_y_end = starty + you_win_dim[1] - 1;

    const int iterations = screen_y + 1;
    const int delay_ms = 100; 
    const int confetti_count = (int) (screen_x * screen_y) * 0.05;  // MAYBE DEPENDENT ON SCREEN DIMENSIONS?
    int** confetti_positions = malloc(confetti_count * sizeof(int*));

    // colors
    init_pair(1, 1, COLOR_BLACK);
    init_pair(2, 2, COLOR_BLACK);
    init_pair(3, 3, COLOR_BLACK);
    init_pair(4, 4, COLOR_BLACK);
    init_pair(5, 5, COLOR_BLACK);
    init_pair(6, 6, COLOR_BLACK);
    init_pair(7, 7, COLOR_BLACK);

    for (int i=0; i<confetti_count; i++) {
      confetti_positions[i] = malloc(3 * sizeof(int));
      int rand_color = (rand() % 7) + 1;
      put_random_coords_except_area(screen_x, screen_y, non_editable_x_start, non_editable_x_end, non_editable_y_start, non_editable_y_end, &confetti_positions[i][1], &confetti_positions[i][0]);
      confetti_positions[i][2] = rand_color;
    }

    for (int iter=0; iter<iterations; iter++) {
      for (int c=0; c<confetti_count; c++) {
        attron(COLOR_PAIR(confetti_positions[c][2]));
        mvaddch(confetti_positions[c][0], confetti_positions[c][1], '*');  
        attroff(COLOR_PAIR(confetti_positions[c][2]));
      }
      refresh();
      sleep_ms(delay_ms);

      if (iter == iterations - 1) break;
      for (int c=0; c<confetti_count; c++) {
        mvaddch(confetti_positions[c][0], confetti_positions[c][1], ' ');  
        confetti_positions[c][0]++;
        int direction = rand() % 3;
        switch (direction) {
          case 0:
            confetti_positions[c][1]--;
            break;

          case 1:
            confetti_positions[c][1]++;
            break;

          case 2:
            break;
          
          default:
            break;
        }

        for (int i = 0; i < you_win_dim[1]; i++) {
          mvprintw(starty + i, startx, "%s", you_win_art[i]);
        }
      }
      refresh();
    }

  } else {  // computer won
    int startx = (int) (screen_x / 2) - (computer_wins_dim[0] / 2);
    int starty = (int) (screen_y / 2) - (computer_wins_dim[1] / 2);
    clear();
    for (int i = 0; i < computer_wins_dim[1]; i++) {
      mvprintw(starty + i, startx, "%s", computer_wins_art[i]);
    } 
    refresh();
  }

  // exit message
  int starty = (int) screen_y - (0.3 * screen_y);
  int startx = (int) (screen_x / 2) - (24 / 2);
  mvprintw(starty, startx, "Press any key to exit...");
  refresh();
  getch();
}