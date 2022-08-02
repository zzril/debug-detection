SRC_PATH=./src/
BUILD_PATH=./build/
BIN_PATH=./bin/

NAME=no_debug
MAIN=main
LGR=logger

BIN=$(BIN_PATH)$(NAME)

SOURCES=$(SRC_PATH)$(MAIN).c $(SRC_PATH)$(NAME).c $(SRC_PATH)$(LGR).c
HEADERS=$(SRC_PATH)$(LGR).h
OBJECTS=$(BUILD_PATH)$(MAIN).o $(BUILD_PATH)$(NAME).o $(BUILD_PATH)$(LGR).o

CC=gcc
CFLAGS=-Wall -Wextra

LD=$(CC)
LDFLAGS=$(CFLAGS)

RM=rm
RFLAGS=

CLEANUP=

CLEAR_BUILD_DIR=$(RM) $(OBJECTS)

ifeq ($(LOGGING),1)
CFLAGS:=$(CFLAGS) -D LOG_ALL_CHECK_RESULTS
# ensure that everything has to be recompiled:
CLEANUP:=$(CLEAR_BUILD_DIR)
endif

ifeq ($(LOGGING),2)
CFLAGS:=$(CFLAGS) -D LOGGER_COLORED_OUTPUT
# ensure that everything has to be recompiled:
CLEANUP:=$(CLEAR_BUILD_DIR)
endif

# --------

all: $(BIN)
	$(CLEANUP)

clean:
	$(RM) $(BIN) $(OBJECTS) || true

# --------

$(BIN): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


