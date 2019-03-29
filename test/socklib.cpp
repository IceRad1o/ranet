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
    hints.ai_family = AF_UNSPEC;    // support IPv4 and IPv6 chioces
    hints.ai_socktype = SOCK_STREAM;  // tcp socket
    hints.ai_flags = AI_PASSIVE ; // all interfaces
    s = getaddrinfo(NULL, strport, &hints, &result);
    if(s!= 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        abort();
    }
    for(rp = result;rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family,rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1)
            continue;
        int optval = 1;
        if(setsockopt(sfd, SOL_SOCKET, reuseport?SO_REUSEPORT:SO_REUSEADDR,&optval, sizeof(int)) < 0)
            continue;
        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if(s == 0){
            break;
        }
        close(sfd);
    }
    if(rp == NULL){
        fprintf(stderr,"could not bind");
        abort();
    }
    freeaddrinfo(result);
    return sfd;
}

void make_socket_non_blocking(int sfd){

}
