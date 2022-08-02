#! /bin/sh -e

COMMAND="./bin/no_debug"
GDB_SCRIPT="./gdb/input.gdb"

LOGGING=2 make clean all
echo
echo "Running normally:"
$COMMAND
echo
echo "Running with \"LINES\" environment variable:"
LINES=1 $COMMAND
echo
echo "Running gdb script:"
gdb -batch -x $GDB_SCRIPT $COMMAND
echo "INFO: Note that the first check always fails when run from a script."
echo


