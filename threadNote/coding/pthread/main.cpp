/*使用 g++编译时 g++ main.cpp -lpthread -o main -l是链接到某个动态库 -o是给编译出来的可执行程序取名*/
#include<iostream>
#include<unistd.h>

#include<pthread.h>

typedef struct thread_data
{
    int num;
    int age;
}tData;

void* callback(void* arg) {

    sleep(1);  //当前线程休眠 头文件为unistd.h
    for(int i=0;i<10;++i) {
        std::cout<<"子线程："<<i<<std::endl;
    }
    std::cout<<"子线程："<<pthread_self()<<std::endl;
    
    // tData t;
    // t.num=100;
    // t.age=20;    临时变量作为返回值 当线程回收后 当前线程栈区内存也被回收 所以这样不行  用new申请堆区内存或者定义成static也行 或者使用全局变量
    
    tData* mainData=static_cast<tData*>(arg);   //将主线程传递过来的参数 转换成应有的类型 并对其进行操作

    mainData->age=100;    
    mainData->num=1000;

    tData* t=new tData;

    t->age=20;
    t->num=100;

    pthread_exit(t);//当前线程退出 且不影响其他线程运行 括号中的参数是线程返回的数据

    return NULL;
}

int main() {

    tData tdata1;  //主线程临时变量 在创建子线程时将其指针传给子线程 因为当前情况主线程会在子线程回收后才回收 
                   //所以可以直接将主线程的栈区临时变量指针传给子线程
    pthread_t tid;   //线程id类型 无符号长整形

    pthread_create(&tid,NULL,callback,&tdata1);  //子线程创建函数
    
    std::cout<<"主线程："<<pthread_self()<<std::endl;  //pthread_self 返回当前线程id
    
    //pthread_exit(NULL); //当前线程退出 且不影响其他线程运行 括号中的参数是线程返回的数据
    
    void* ptr;
    pthread_join(tid,&ptr); //线程回收函数 等待tid的线程退出 并回收其返回值 第二个参数是一个void**的二级指针

    tData* tdata2=static_cast<tData*>(ptr);  //使用类型转换

    std::cout<<"tdata1.num:"<<tdata1.num<<std::endl;
    std::cout<<"tdata1.age:"<<tdata1.age<<std::endl;

    std::cout<<"tdata2->num:"<<tdata2->num<<std::endl;
    std::cout<<"tdata2->age:"<<tdata2->age<<std::endl;

    delete tdata2;

    for(int i=0;i<10;++i) {
        std::cout<<"主线程："<<i<<std::endl;
    }
    std::cout<<"主线程："<<pthread_self()<<std::endl;
    

    return 0;

}