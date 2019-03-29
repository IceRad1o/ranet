#pragma once

#include <sys/types.h>
int create_and_bind(int port,bool reuseport);
void make_socket_non_blocking(int socket_fd);
ssize_t rio_readn(int fd, void *userbuf, size_t n);
ssize_t rio_writen(int fd, const void *userbuf, size_t);
int accept_connection(int socket_fd);
void tcp_cork_on(int fd);
void tcp_cork_off(int fd);
void tcp_nodelay_on(int fd);
void tcp_nodelay_off(int fd);

