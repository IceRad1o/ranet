#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
// #include <sys/epoll.h>
#include <netinet/tcp.h>

#include "socklib.hpp"

int create_and_bind(int port,bool reuseport){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;
    char strport[10];
    snprintf(strport, 10, "%d", port);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
}
