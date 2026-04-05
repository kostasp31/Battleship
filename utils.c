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
