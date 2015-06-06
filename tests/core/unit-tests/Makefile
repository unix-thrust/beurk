SHELL	:= /bin/bash
TARGET	= test

ROOTDIR	= $(shell git rev-parse --show-toplevel)

CFLAGS	:= -I$(ROOTDIR)/includes -Wall -Wextra
LDLIBS	:= -lselinux
LDFLAGS	:= -L$(ROOTDIR)

CONFIG	= $(ROOTDIR)/src/config.c
SRC		= $(wildcard *.c)
OBJ_DIR	= objs
OBJ		= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o) config.o)

.PHONY: all clean distclean re

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/config.o: $(CONFIG)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

distclean: clean
	-$(RM) $(TARGET)

re: distclean all

