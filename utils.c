// Source - https://stackoverflow.com/a/28827188
// Posted by Bernardo Ramos, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-05, License - CC BY-SA 4.0

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#include <time.h>
#endif

#include <stdlib.h>

// cross-platform sleep function
void sleep_ms(int milliseconds) {
  #ifdef WIN32
      Sleep(milliseconds);
  #elif _POSIX_C_SOURCE >= 199309L
      struct timespec ts;
      ts.tv_sec = milliseconds / 1000;
      ts.tv_nsec = (milliseconds % 1000) * 1000000;
      nanosleep(&ts, NULL);
  #else
      if (milliseconds >= 1000)
        sleep(milliseconds / 1000);
      usleep((milliseconds % 1000) * 1000);
  #endif
}

// random number smaller than max excepr range
void put_random_coords_except_area(int max_x, int max_y, int range_x_start, int range_x_end, int range_y_start, int range_y_end, int* res_x, int* res_y) {
  int rand_x = rand() % max_x;
  int rand_y = rand() % max_y;
  while ((rand_x >= range_x_start && rand_x <= range_x_end) && (rand_y >= range_y_start && rand_y <= range_y_end)) {
    rand_x = rand() % max_x;
    rand_y = rand() % max_y;
  };

  *res_x = rand_x;
  *res_y = rand_y;

  return;
}
