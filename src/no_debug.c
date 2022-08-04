#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "logger.h"

// --------

#ifndef FALSE
	#define FALSE (0 != 0)
#endif

#define INT3 '\xCC'

// --------

typedef int (*Check)();

// --------

// Extern function addresses:
// (needed in order to check for breakpoints)

extern void _start();
extern int main();

// --------

// static function declarations:

/**
 * Executed before `main()`, checks for presence of a debugger.
 * If no debugger is found, executes the "bad functionality".
 */
static __attribute__ ((constructor)) void pre();

/**
 * Executes the "bad functionality".
 * (In this dummy example, that is just printing a string.)
 * Called when the absence of a debugger was confirmed.
 */
static void execute_bad_functionality();

/**
 * Returns nonzero iff the `_` environment variable ends with "/gdb".
 */
static int is_run_with_gdb_command();

/**
 * Returns nonzero iff environment variables `LINES` or `COLUMNS` are found.
 */
static int has_fishy_env_vars();

/**
 * Returns nonzero iff a software breakpoint was detected.
 */
static int has_breakpoints();

/**
 * Performs all of the above checks.
 * Returns nonzero iff the presence of a debugger was found.
 */
static int is_debugged();

// --------

// static function implementations:

static __attribute__ ((constructor)) void pre() {
	if(!is_debugged()) {
		execute_bad_functionality();
	}
	return;
}

static void execute_bad_functionality() {
	printf("Executing bad functionality...\n");
	return;
}

static int is_run_with_gdb_command() {

	// get `_` variable (last executed command):
	const char* last_command = getenv("_");

	if(last_command == NULL) {	// maybe this could also
		return 0;		// be considered fishy?
	}
	const size_t last_command_len = strlen(last_command);

	// compare last 4 chars of last command with "/gdb":
	const char* gdb = "/gdb";
	const size_t index = last_command_len - 4;
	const int ends_without_gdb = strcmp(last_command + index, gdb);

	logger_f(ends_without_gdb, "Last command %s with \"/gdb\".\n",
		(ends_without_gdb ? "does not end" : "ends"));

	return !ends_without_gdb;
}

static int has_fishy_env_vars() {

	// define fishy variables:
	char* fishy_env_vars[] = {
		"COLUMNS",
		"LINES"
	};
	const size_t num_fishy_env_vars = sizeof(fishy_env_vars)/sizeof(char*);

	int fishy_env_detected = 0;

	// iterate through given vars:
	for(size_t i=0; i<num_fishy_env_vars; i++)  {

		// check if env var exists:
		int result = getenv(fishy_env_vars[i]) != NULL;

		logger_f(!result, "Environment variable \"%s\" was %sfound.\n",
			fishy_env_vars[i], (result ? "" : "not "));

		// update flag:
		fishy_env_detected |= result;
	}

	return fishy_env_detected;
}

static int has_breakpoints() {

	// Addresses of typical breakpoint targets:
	// (remember to update this with any new functions added!)
	const char* function_addresses[] = {
		(char*) _start,
		(char*) main,
		(char*) pre,
		(char*) is_run_with_gdb_command,
		(char*) has_fishy_env_vars,
		(char*) has_breakpoints,
		(char*) is_debugged
	};
	const size_t num_functions = sizeof(function_addresses)/sizeof(char*);

	int num_breakpoints = 0;
	int has_breakpoints = 0;

	// check beginning of each function for breakpoints:
	for(size_t i=0; i<num_functions; i++)  {

		if(*(function_addresses[i]) == INT3) {	// interrupt inserted
							// by a debugger

			logger_f(FALSE, "Breakpoint at start of function #%d "
				"detected.\n", i+1);

			// increment breakpoint counter
			num_breakpoints++;
			has_breakpoints = -1;	// this will be set even if
						// `num_breakpoints` overflows
		}
	}

	logger_f(!has_breakpoints, "%d software interrupt(s) found.\n",
		num_breakpoints);

	// return `num_breakpoints`,
	// or `has_breakpoints` if the former one overflowed:
	return num_breakpoints != 0 ? num_breakpoints : has_breakpoints;
}

static int is_debugged() {

	// define checks:
	Check checks[] = {
		is_run_with_gdb_command,
		has_fishy_env_vars,
		has_breakpoints
	};
	const size_t num_checks = sizeof(checks)/sizeof(char*);

	int debugger_present = 0;

	// iterate through all checks:
	for(size_t i=0; i<num_checks; i++)  {

		int result = checks[i]();

		debugger_present |= result;
	}

	logger_f(!debugger_present, "All checks finished. %s found.\n",
		(debugger_present ? "Debugger" : "No debugger"));

	return debugger_present;
}

// --------


