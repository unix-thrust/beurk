SHELL	:= /bin/bash
TARGET	= unit-tests

ROOTDIR	= $(shell git rev-parse --show-toplevel)
LIB		= $(ROOTDIR)/libselinux.so

CFLAGS	:= -I$(ROOTDIR)/includes -Wall -Wextra
LDLIBS	:= -lselinux
LDFLAGS	:= -L$(ROOTDIR)

CONFIG	= $(ROOTDIR)/src/config.c
SRC		= $(wildcard *.c)
OBJ_DIR	= objs
OBJ		= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o) config.o)

.PHONY: all clean distclean re

all: $(LIB) $(OBJ_DIR) $(TARGET)

$(LIB):
	BEURK_DEBUG_LEVEL=1 $(MAKE) -C $(ROOTDIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/config.o: $(CONFIG)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

distclean: clean
	$(RM) $(TARGET)

re: distclean all

