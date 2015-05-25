#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    struct stat buf;
    lstat("tata", &buf);
}
