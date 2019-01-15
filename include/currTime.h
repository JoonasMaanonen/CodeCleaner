#ifndef CURRTIME_H
#define CURRTIME_H

// This function returns a string that contains the current-time in the format
// define by the function argument. This format is the format used by strftime()
// function. See man strftime for more information about the format.
char * currTime(const char* format);


#endif

