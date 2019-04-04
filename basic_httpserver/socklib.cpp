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

ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    // char *bufp = usrbuf it's ok with c but not with c++
    char *bufp = (char*)usrbuf;
    while(nleft>0)
    {
        if((nread = read(fd, bufp, n)) < 0 )
        {
            if(errno == EINTR)   // Interrupted by sig handler return
                nread = 0;      // call read() again
            else
                return -1;      // errno set by read()
        }
        else if(nread == 0)
            break;              // EOF
        nleft -= nread;
        bufp += nread;
    }
    return n-nread;
}


ssize_t rio_writen(int fd, void*usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char*)usrbuf;
    while(nleft > 0)
    {
        if((nwritten = write(fd, bufp, n)) <= 0)
        {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

void readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, void*userbuf, size_t n)
{
    return 0;
}

/*
ssize_t readlineb(rio_t *rp, void *usrbuf ,size_t maxlen);
ssize_t readnb(rio *rp, void *usrbuf, size_t n);
*/

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
    /*
     * getaddrinfo(const char *node,const char *service, cosnt struct addrinfo*hits, struct addrinfo **res);
     * server often set node as NULL so return [::]
     * the following is from linux man page---about how server typically do.
     * If the AI_PASSIVE flag is specified in hints.ai_flags, and node is
     * NULL, then the returned socket addresses will be suitable for
     * bind(2)ing a socket that will accept(2) connections.  The returned
     * socket address will contain the "wildcard address" (INADDR_ANY for
     * IPv4 addresses, IN6ADDR_ANY_INIT for IPv6 address).  The wildcard
     * address is used by applications (typically servers) that intend to
     * accept connections on any of the host's network addresses.  If node
     * is not NULL, then the AI_PASSIVE flag is ignored.*/
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

