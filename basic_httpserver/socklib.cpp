#include "socklib.h"

// this is a simple example for binding and listening
int create_and_bind_raw(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        return -1;
    listen(listenfd, 1024);
    return listenfd;
}


// trying to use setsockopt to reuse the port immediately otherwise the connection will enter a TIME_WAIT
int create_and_bind(int port,bool reuseport){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;
    char strport[10];
    snprintf(strport, 10, "%d", port);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;      // support IPv4 and IPv6 chioces
    hints.ai_socktype = SOCK_STREAM;  // tcp socket
    hints.ai_flags = AI_PASSIVE ;     // all interfaces

    //getaddrinfo(const char *node,const char *service, cosnt struct addrinfo*hits, struct addrinfo **res)
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

    /* Make it a listening socket ready to accept connection requests */
    if (listen(sfd, 5) < 0)
        return -1;
    return sfd;
    // why there isn't a listen function?
}



void make_socket_non_blocking(int sfd){
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if(flags == -1){
        perror("fcntl");
        abort();
    }
    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if(s == -1){
        perror("fcntl");
        abort();
    }
}

int accept_connection(int sfd)
{
    int infd = accept(sfd, NULL, NULL);
    return infd;
}

int accept_conn(int fd, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;
    if((rc = accept(fd, addr, addrlen)) < 0)
        perror("accept error");
    return rc;
}
/*
int accept_connection_linux(int sfd)
{
    int infd = accept4(sfd, NULL, NULL, SOCK_NONBLOCK);
    if(infd < 0){
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            return -1;
        }
        else{
            perror("accept4");
            abort();
        }
    }
    return infd;
}
*/

/*
 * current don't set tcp opt on MAC OS
static void set_tcp_opt(int fd, int opt, int optval)
{
    if(setsockopt(fd, SOL_TCP, opt, &optval, sizeof(optval)) < 0){
        perror("setsockopt");
        abort();
    }
}
*/
void tcp_cork_on(int fd);

void tcp_cork_off(int fd);

void tcp_nodelay_on(int fd);

void tcp_nodelay_off(int fd);

