#pragma once
#include <sys/types.h>

#define RIO_BUFSIZE 8192
typedef struct{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;

// <cmu/csapp>:robust io.The function is inspired from UNP:ch3 readn/writen/readline
//rio_readn and rio_writen are rio's io functions without buffer,it's easy to transfer data between memory and file
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *userbuf, size_t n);

//rio buffered io functions
//readnb can be used to for both text file and binary file while readlineb is just for text file
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);

int create_and_bind(int port,bool reuseport);
void make_socket_non_blocking(int socket_fd);
int accept_connection(int socket_fd);
void tcp_cork_on(int fd);
void tcp_cork_off(int fd);
void tcp_nodelay_on(int fd);
void tcp_nodelay_off(int fd);

