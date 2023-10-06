#include<iostream>
#include<pthread.h>
#include<unistd.h>

#define MAX 5

pthread_mutex_t mutex;
pthread_cond_t cond;

class node {

public:
    node(){ conut++;}
    ~node(){ conut--; }
    int number;
    node* next;

    static int getConut(){ return conut;}

    static int conut;
};

int node::conut=0;

node* head;

void* producer(void* arg) {

    while(true) {
        pthread_mutex_lock(&mutex);
        while(node::getConut()==MAX) {
            pthread_cond_wait(&cond,&mutex);
        }
        node* newNode=new node;
        newNode->number=rand()%1000;
        newNode->next=head;
        head=newNode;
        std::cout<<"生产者： "<<pthread_self()<<" 产品编号： "<<newNode->number<<" 商品存量: "<<node::getConut()<<std::endl;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
        sleep(rand()%5);
    }

    return NULL;
}

void* consumer(void* arg) {
 
    while(true) {
        pthread_mutex_lock(&mutex);
        while(!head) {
            pthread_cond_wait(&cond,&mutex);
        }
        node* hNode=head;
        std::cout<<"消费者： "<<pthread_self()<<" 产品编号： "<<hNode->number<<" 商品存量: "<<node::getConut()-1<<std::endl;
        head=hNode->next;
        delete hNode;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
        sleep(rand()%5);
    }

    return NULL;
}

int main() {

    srand((unsigned)time(NULL));

    pthread_t tidP[5],tidC[5];

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    for(int i=0;i<5;++i) {
        pthread_create(&tidP[i],NULL,producer,NULL);
    }

    for(int i=0;i<5;++i) {
        pthread_create(&tidC[i],NULL,consumer,NULL);
    }

    for(int i=0;i<5;++i) {
        pthread_join(tidC[i],NULL);
        pthread_join(tidP[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
