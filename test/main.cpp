#include <iostream>
#include <pthread.h>
#include "threadpool.hpp"
#include "socklib.hpp"
pthread_t ntid;
void *work(void *arg){
    std::cout<<"pthread test"<<std::endl;
}
int main() {
    std::cout << "Hello, World!" << std::endl;
    int err = pthread_create(&ntid,NULL,work,NULL);
    std::vector<pthread_t> test(10);

    std::cin.get();
    return 0;
}