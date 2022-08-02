#ifndef LOGGER_H
#define LOGGER_H
// --------

/**
 * Performs logging in `printf()` style.
 * Prints in green iff success is nonzero, otherwise uses default color.
 */
int logger_f(int success, const char* format, ...);

// --------
#endif


