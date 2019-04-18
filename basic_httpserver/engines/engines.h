//
// Created by Jialei Wang on 2019/4/4.
//

#ifndef BASIC_HTTPSERVER_ENGINES_H
#define BASIC_HTTPSERVER_ENGINES_H

void engine_bruteforce(int sfd, int backlog, int _);

void engine_fork(int sfd, int backlog, int _);

void engine_thread(int sfd, int backlog, int _);

void engine_select(int sfd, int backlog, int _);

void engine_poll(int sfd, int backlog, int _);


#endif //BASIC_HTTPSERVER_ENGINES_H
