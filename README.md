debug-detection
===============

Just trying out some ways to detect debugging from within a debugged program.

Currently, these checks are run:
* test whether environment variable `_` ends with `/gdb`
* checks for existence of environment variables `LINES` or `COLUMNS` (`gdb` adds those)
* checks for breakpoints (`INT3` instruction / `\xCC` opcode) inserted at the start of a function

The `_` environment variable is maintained by the shell and means sthg like "last user-run command".  
It is only updated when the user runs a new command, not for commands run within another program.  
Therefore, this check fails when `gdb` is not invoked by the user directly, but run from a script.

On some systems, `gdb` seems to insert its breakpoints *after* the function prologue.  
These are currently not detected.

---

Run the `no_debug` program:
---------------------------

### Run normally:

Compile with `make`, run with `./bin/no_debug`.  
Or type `./run.sh`.

Run in gdb with `make && gdb ./bin/no_debug`.

---

### Run with logging:

In order to enable logging, compile with:  
`LOGGING=2 make clean all`.  
When compiled this way, the program will print the results of the various  
checks for the presence of a debugger along the way.

Alternatively, type `./test.sh` to compile with logging enabled and run some  
standard tests inside and outside gdb.

If the terminal does not support ANSI escape codes, use:  
`LOGGING=1 make clean all`


