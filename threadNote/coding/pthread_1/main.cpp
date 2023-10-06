#include<iostream>
#include<unistd.h>
#include<pthread.h>

#define MAX 5
int cur=0;

pthread_mutex_t mutex;

void* funcA(void* arg) {
      
   for(int i=0;i<MAX;++i) {
       pthread_mutex_lock(&mutex);
       int num=cur;
       num++;
       usleep(10);
       cur=num;
       std::cout<<"thread1: "<<pthread_self()<<" cur: "<<num<<std::endl;
       pthread_mutex_unlock(&mutex);
       usleep(10);
   }

   return NULL;
}

void* funcB(void* arg) { 
      
   for(int i=0;i<MAX;++i) {
       pthread_mutex_lock(&mutex);
       int num=cur;
       num++;
       usleep(10);
       cur=num;
       std::cout<<"thread2: "<<pthread_self()<<" cur: "<<num<<std::endl; 
       pthread_mutex_unlock(&mutex);
       usleep(10);
   }

   return NULL;
}

//互斥锁
void test_mutex() {

    pthread_t tid1,tid2;

    pthread_mutex_init(&mutex,NULL);

    pthread_create(&tid1,NULL,funcA,NULL);
    pthread_create(&tid2,NULL,funcB,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_mutex_destroy(&mutex);

}

pthread_rwlock_t rwlock;

void* func1(void* arg) {

    for(int i=0;i<MAX;++i) {
       pthread_rwlock_wrlock(&rwlock);
       int num=cur;
       num++;
       usleep(10);
       cur=num;
       std::cout<<"thread1: "<<pthread_self()<<" cur: "<<num<<std::endl; 
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;

}

void* func2(void* arg) {

    for(int i=0;i<MAX;++i) {
       pthread_rwlock_wrlock(&rwlock);
       int num=cur;
       num++;
       usleep(10);
       cur=num;
       std::cout<<"thread2: "<<pthread_self()<<" cur: "<<num<<std::endl; 
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void* func3(void* arg) {

    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread3: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl; 
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void* func4(void* arg) {

    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread4: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl; 
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void* func5(void* arg) {

    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread5: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl;  
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
    
}

void* func6(void* arg) {
    
    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread6: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl;  
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void* func7(void* arg) {
    
    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread7: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl; 
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void* func8(void* arg) {
    
    for(int i=0;i<MAX;++i) {
       pthread_rwlock_rdlock(&rwlock);
       int num=cur;
       usleep(10);
       std::cout<<"thread8: "<<pthread_self()<<" cur: "<<num<<" i: "<<i<<std::endl;  
       pthread_rwlock_unlock(&rwlock);
       usleep(10);
   }

   return NULL;
}

void test_rwlock() {

    pthread_t tid1,tid2,tid3,tid4,tid5,tid6,tid7,tid8;

    pthread_rwlock_init(&rwlock,NULL);


    pthread_create(&tid1,NULL,func1,NULL);
    pthread_create(&tid2,NULL,func2,NULL);
    pthread_create(&tid3,NULL,func3,NULL);
    pthread_create(&tid4,NULL,func4,NULL);
    pthread_create(&tid5,NULL,func5,NULL);
    pthread_create(&tid6,NULL,func6,NULL);
    pthread_create(&tid7,NULL,func7,NULL);
    pthread_create(&tid8,NULL,func8,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    pthread_join(tid5,NULL);
    pthread_join(tid6,NULL);
    pthread_join(tid7,NULL);
    pthread_join(tid8,NULL);

    pthread_rwlock_destroy(&rwlock);

}

int main() {
       
    //test_mutex();

    test_rwlock();

    return 0;

}