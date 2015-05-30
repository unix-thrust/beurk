SHELL	:= /bin/bash
TARGET	= libtest

ROOTDIR	= $(shell git rev-parse --show-toplevel)

CFLAGS	:= -I$(ROOTDIR)/includes -Wall -Wextra
LDFLAGS	:= -L$(ROOTDIR) -lselinux

CONFIG	= $(ROOTDIR)/src/config.c
SRC		= $(wildcard *.c)
OBJ_DIR	= objs
OBJ		= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o) config.o)

.PHONY: all clean distclean re

all: $(OBJ_DIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/config.o: $(CONFIG)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	-rm -f $(OBJ)

distclean: clean
	-rm -f $(TARGET)

re: distclean all

