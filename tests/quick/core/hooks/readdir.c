#include <sys/types.h>
#include <dirent.h>

int     main(void) {
    DIR *tmp;

    tmp = opendir("../");
    readdir(tmp);
}
