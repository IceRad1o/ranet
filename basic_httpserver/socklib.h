#pragma once
#include <sys/types.h>
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

int create_and_bind(int port,bool reuseport);

void make_socket_non_blocking(int socket_fd);

int accept_connection(int socket_fd);
int accept_conn(int fd, struct sockaddr*, socklen_t*);
// some opt on tcp performance used on linux
void tcp_cork_on(int fd);
void tcp_cork_off(int fd);
void tcp_nodelay_on(int fd);
void tcp_nodelay_off(int fd);

