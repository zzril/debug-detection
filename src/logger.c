#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

// --------

#ifdef LOGGER_COLORED_OUTPUT
	#ifndef LOG_ALL_CHECK_RESULTS
		#define LOG_ALL_CHECK_RESULTS
	#endif
	#define LOGGER_START "\x1b[31m"
	#define LOGGER_END "\x1b[0m"
	#define COLOR_SUCCESS "\x1b[32m"
#else
	#define LOGGER_START ""
	#define LOGGER_END ""
	#define COLOR_SUCCESS ""
#endif

#ifdef LOG_ALL_CHECK_RESULTS

#ifndef LOGGING_STREAM
	#define LOGGING_STREAM stderr
#endif

int logger_f(int success, const char* format, ...) {

	int chars_printed = 0;

	va_list ap;
	va_start(ap, format);

	// put color prefix:
	fputs(LOGGER_START, LOGGING_STREAM);
	if(success) {
		fputs(COLOR_SUCCESS, LOGGING_STREAM);
	}

	// pass call to `vfprintf()`:
	chars_printed =  vfprintf(LOGGING_STREAM, format, ap);

	// put color postfix:
	fputs(LOGGER_END, LOGGING_STREAM);

	va_end(ap);

	return chars_printed;
}

#else

// no logging - define empty logger function and ignore compiler warning:

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int logger_f(int success, const char* format, ...) {
	return 0;
}

#pragma GCC diagnostic pop

#endif

// --------


