#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int        main(int ac, char **av) {
    if (ac == 2) {
        open(av[1], atoi(av[2]));
    }
    else if (ac > 3) {
        open(av[1], atoi(av[2]), atoi(av[3]));
    }
    return 0;
}
