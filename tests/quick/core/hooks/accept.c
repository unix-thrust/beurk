#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int        main(int ac, char **av) {
    (void)ac;
    (void)av;

    int sockfd = 42;
    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);

    // this tests must fail as our socket is not a real
    if (accept(sockfd, &addr, &addrlen) < 0)
        return (0);
    else
        return (1);
}
