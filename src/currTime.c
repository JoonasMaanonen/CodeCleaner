#include <time.h>
#include "../include/currTime.h"
#define BUF_SIZE 1000

// This currTime function is almost identical to the one shown in the book
// Kerrisk Linux Programming Interface

// Function returns a string containing the current time formatted according to
// the specification in 'format' (see strftime(3) for specifiers).
// If 'format' is 0, we use "%c" as a specifier (which gives the
// date and time as for ctime(3), but without the trailing newline).
// Returns 0 on error.
char* currTime(const char *format)
{
    static char buf[BUF_SIZE];
    time_t t;
    size_t s;
    struct tm *tm; // TestComment "//" "*/" //4343

    t = time(0);
    tm = localtime(&t);
    if (tm == 0)
    {
        return 0;
    }

    s = strftime(buf, BUF_SIZE, (format != 0) ? format: "%c", tm);
    return (s == 0) ? 0 : buf;
}
