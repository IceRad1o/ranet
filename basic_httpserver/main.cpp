#include "engines/engines.h"


int main(int argc, char **argv) {
    int listenfd, port;
    if(argc!=2){
        fprintf(stderr, "usage: %s<port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    listenfd = create_and_bind(port, true);
    native_engine(listenfd);

}
