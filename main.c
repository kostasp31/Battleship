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

#include "ncurses_moves.h"

#include "ascii_art.h"

#include <ncurses.h>
#include <panel.h>

#define DELAY 500  // ms in printing delay

// SEE: https://github.com/mcdaniel/curses_tutorial
// FOR ART: https://ascii.co.uk/art/battleship
// https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/

// TODO: smarter AI
int main() {
  initscr();
  if(has_colors() == FALSE) {	
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  noecho(); // Don't echo input characters to the screen
  keypad(stdscr, TRUE); // Enable special keys (arrow keys, F1-F12, etc.)
  curs_set(1);  // 0 for invisible cursor, 1 visible
  cbreak(); // respect Ctrl C, Ctrl Z signals (else use raw())
	start_color();

  int screen_x, screen_y; // terminal dimensions
  MEVENT event;
  WINDOW *title_win, *log_win, *board_win;  // 2 windows: 1 for the instructions, 1 for the boards

  // mouse stuff
  mmask_t mask, oldmask;
  mask = BUTTON1_CLICKED | BUTTON3_CLICKED;
  mousemask(mask, &oldmask); // Assigns new mask and saves previous one
  mouseinterval(0); // No delay for mouse clicks

  getmaxyx(stdscr, screen_y, screen_x); // get terminal dimensions
  title_win = newwin(screen_y, screen_x, 0, 0);
  keypad(title_win, TRUE);      
  box(title_win, 0 , 0);	
  wrefresh(title_win);
  
  srand(time(NULL));
  Board* player1_board = initialize_board();
  Board* player2_board = initialize_board();

  Player* player1 = initialize_player("Player", player1_board);
  Player* player2 = initialize_player("Computer", player2_board);
  Player* players[2] = { player1, player2 };

  
  extern const char *ship_art[];
  extern const char *title_art[];
  extern const char *start_button_art[];
  extern const char *starting_message_art[3][8];
  extern const char* place_your_fleet_art[];
  extern const char* game_begins_art[];
  extern const char* you_win_art[];
  extern const char* computer_wins_art[];
  extern const int ship_art_dim[];
  extern const int title_art_dim[];
  extern const int start_button_art_dim[];
  extern const int starting_message_art_dim[];
  extern const int place_your_fleet_art_dim[];
  extern const int game_begins_dim[];
  extern const int you_win_dim[];
  extern const int computer_wins_dim[];
  
  // print warship
  int starty = (int) (0.1 * screen_y);
  int startx = (int) (screen_x / 2) - (ship_art_dim[0] / 2);
  for (int i = 0; i < ship_art_dim[1]; i++) {
    mvwprintw(title_win, starty + i, startx, "%s", ship_art[i]);
  }
  
  // print title
  starty += 13;
  startx = (int) (screen_x / 2) - (title_art_dim[0] / 2);
  for (int i = 0; i < title_art_dim[1]; i++) {
    mvwprintw(title_win, starty + i, startx, "%s", title_art[i]);
  }

  // print start button
  starty = (int) screen_y - (0.15 * screen_y);
  startx = (int) (screen_x / 2) - (start_button_art_dim[0] / 2);
  for (int i = 0; i < start_button_art_dim[1]; i++) {
    mvwprintw(title_win, starty + i, startx, "%s", start_button_art[i]);
  }
  wrefresh(title_win);
  
  int ch;
  int in_title_screen = 1;
  while (in_title_screen) {
    ch = wgetch(title_win);
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
  box(title_win, 0, 0);

  startx = (int) (screen_x / 2) - (starting_message_art_dim[0] / 2);
  starty = (int) (screen_y / 2) - (starting_message_art_dim[1] / 2);
  for (int iter = 0; iter< 3; iter++) {
    for (int i = 0; i < starting_message_art_dim[1]; i++) {
      mvwprintw(title_win, starty + i, startx, "%s", starting_message_art[iter][i]);
    }
    wrefresh(title_win);
    sleep_ms(150);
  }

  startx = (int) (screen_x / 2) - (place_your_fleet_art_dim[0] / 2);
  starty = (int) (screen_y / 2) - (place_your_fleet_art_dim[1] / 2);
  for (int i = 0; i < starting_message_art_dim[1]; i++) {
    mvwprintw(title_win, starty + i, startx, "%s", place_your_fleet_art[i]);
  }
  wclear(title_win);
  wrefresh(title_win);
  wdelch(title_win);

  // delete title window and create new regions 
  // +--------+---------------+
  // |  LOGS  |     BOARDS    |
  // +--------+---------------+
  // log window: 20%
  int log_win_width = (int) (0.2 * screen_x);
  int log_win_height = screen_y;
  log_win = newwin(log_win_height, log_win_width, 0, 0);
  keypad(log_win, TRUE); 
  box(log_win, 0, 0);
  wrefresh(log_win);

  // board window: 80%
  int board_win_width = screen_x - log_win_width;
  int board_win_height = screen_y;
  board_win = newwin(board_win_height, board_win_width, 0, log_win_width);
  keypad(board_win, TRUE); 
  box(board_win, 0, 0);

  // // FIRST GAME STAGE: Ship placement. Auto for Computer, using the UI for the user
  // char orientation = 'H';
  // int ships[] = { 5, 4, 3, 3, 2 };
  // for (int i=0; i<2; i++) {
  //   for (int j=0; j<5; j++) {
  //     if (strncmp(players[i]->name, "Computer", 8) == 0) {
  //       auto_place_ship(players[i]->board, ships[j]);
  //     } else {
  //       ncurses_place_ships(board_win, ships, j, board_win_height, board_win_width, players[i]->board, &orientation, log_win_width, &event);
  //     }
  //   }
  // }

  // continue button
  starty = (int) screen_y - (0.1 * screen_y);
  startx = (int) (log_win_width + 2) + (board_win_width / 2) - (28 / 2);
  mvwprintw(board_win, starty, startx, "Press any key to continue...");
  wrefresh(board_win);
  
  
  // Print Game begins art
  startx = (int) (board_win_width / 2) - (game_begins_dim[0] / 2);
  starty = (int) (board_win_height / 2) - (game_begins_dim[1] / 2);
  wclear(board_win);
  box(board_win, 0, 0);
  for (int i = 0; i < game_begins_dim[1]; i++) {
    mvwprintw(board_win, starty + i, startx, "%s", game_begins_art[i]);
  }
  wrefresh(board_win);
  
  sleep_ms(250);
  
  // who plays first (index in players list)
  int turn = rand() % 2;

  // clear win, redraw borders
  wclear(board_win);
  box(board_win, 0, 0);

  mvwprintw(board_win, 0, 1, "%s plays first.", players[turn]->name);
  wrefresh(board_win);

  // main game loop, returns index of players[] winner
  int retVal = 0; // ncurses_play_game(board_win, &event, players, turn, board_win_height, board_win_width, log_win_width, 10);


  wclear(board_win);
  wrefresh(board_win);
  wdelch(board_win);
  wclear(log_win);
  wrefresh(log_win);
  wdelch(log_win);

  // print winner message
  if (retVal == 0) {  // player won
    startx = (int) (screen_x / 2) - (you_win_dim[0] / 2);
    starty = (int) (screen_y / 2) - (you_win_dim[1] / 2);
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
    const int confetti_count = 300;  // MAYBE DEPENDENT ON SCREEN DIMENSIONS?
    int** confetti_positions = malloc(confetti_count * sizeof(int*));
    for (int i=0; i<confetti_count; i++) {
      confetti_positions[i] = malloc(2 * sizeof(int));
      put_random_coords_except_area(screen_x, screen_y, non_editable_x_start, non_editable_x_end, non_editable_y_start, non_editable_y_end, &confetti_positions[i][1], &confetti_positions[i][0]);
    }

    // colors
    init_pair(1, 1, COLOR_BLACK);
    init_pair(2, 2, COLOR_BLACK);
    init_pair(3, 3, COLOR_BLACK);
    init_pair(4, 4, COLOR_BLACK);
    init_pair(5, 5, COLOR_BLACK);
    init_pair(6, 6, COLOR_BLACK);
    init_pair(7, 7, COLOR_BLACK);

    for (int iter=0; iter<iterations; iter++) {
      for (int c=0; c<confetti_count; c++) {
        int rand_color = (rand() % 7) + 1;

        attron(COLOR_PAIR(rand_color));
        mvaddch(confetti_positions[c][0], confetti_positions[c][1], '*');  
        attroff(COLOR_PAIR(rand_color));
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
    startx = (int) (screen_x / 2) - (computer_wins_dim[0] / 2);
    starty = (int) (screen_y / 2) - (computer_wins_dim[1] / 2);
    wclear(board_win);
    box(board_win, 0, 0);
    for (int i = 0; i < computer_wins_dim[1]; i++) {
      mvprintw(starty + i, startx, "%s", computer_wins_art[i]);
    } 
  }
  refresh();


  // exit message
  starty = (int) screen_y - (0.3 * screen_y);
  startx = (int) (screen_x / 2) - (24 / 2);
  mvprintw(starty, startx, "Press any key to exit...");
  refresh();
  getch();


  destroy_board(player1_board);
  destroy_board(player2_board);

  destroy_player(player1);
  destroy_player(player2);

  endwin();
  echo();
  curs_set(1);
}