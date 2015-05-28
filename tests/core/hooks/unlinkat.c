#include <unistd.h>

int main(void) {
    unlinkat(42, "foo", 0);
}
