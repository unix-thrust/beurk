#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int        main(int ac, char **av) {
    if (ac == 3) {
        return open(av[1], atoi(av[2]));
    }
    else if (ac > 3) {
        return open(av[1], atoi(av[2]), atoi(av[3]));
    }
    else
        return 1;
    return 0;
}
