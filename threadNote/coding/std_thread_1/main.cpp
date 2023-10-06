#include<iostream>
#include<thread>
#include<pthread.h>
#include<string>
#include<unistd.h>

void funcForStd(std::string str,int count) {
    for(int i=0;i<count;++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<"子线程："<<std::this_thread::get_id()<<" "<<str<<std::endl;
    }
}

struct tData {
    int count;
    std::string str;
};

void* funcForPthread(void* arg) {
    tData* data=static_cast<tData*>(arg);
    for(int i=0;i<data->count;++i) {
        sleep(1);
        std::cout<<"子线程："<<pthread_self()<<" "<<data->str<<std::endl;
    }
    return NULL;
}

int main() {
    int num=std::thread::hardware_concurrency();
    std::cout<<num<<std::endl;

    std::thread th1(funcForStd,static_cast<std::string>("hello std::thread!!"),5);

    pthread_t tid1;
    tData data;
    data.count=5;
    data.str="hello pthread!!";
    pthread_create(&tid1,NULL,funcForPthread,&data);

    th1.join();
    pthread_join(tid1,NULL);

    return 0;
}