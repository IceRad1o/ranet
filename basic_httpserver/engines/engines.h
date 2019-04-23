//
// Created by Jialei Wang on 2019/4/4.
//

#ifndef BASIC_HTTPSERVER_ENGINES_H
#define BASIC_HTTPSERVER_ENGINES_H

#include "../http_request.h"
#include "../socklib.h"

class engine_cmd{

};

void native_engine(int sfd);

void fork_engine(int sfd);


#endif //BASIC_HTTPSERVER_ENGINES_H
