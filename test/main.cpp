#include <iostream>
#include <pthread.h>
#include <string.h>
#include "threadpool.hpp"
#include "socklib.hpp"

using namespace std;
class A{
public:
    A(){
        cout<<"constructor"<<endl;
    }
    ~A(){
        cout<<"destructor"<<endl;
    }
};

pthread_t ntid;

void *work(void *arg){
    std::cout<<"pthread test"<<std::endl;
}

int main() {
   // int err = pthread_create(&ntid,NULL,work,NULL);
    //std::vector<pthread_t> test(10);
    string s1 = string("string1");
    string s2 = string("string2");
    const char *p1 =s1.substr(1).data();
    const char *p2 =s2.substr(1).data();
    cout<<p1<<p2<<endl;
    A(),
    cout<<"end xxx\n";
    cout<<"end yyy\n";
    std::cin.get();
    return 0;
}
