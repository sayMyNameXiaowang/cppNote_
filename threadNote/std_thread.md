# thread头文件
## std::thread

c++11之后提供了多线程相关函数，其中std::thread则是多线程操作类
### 构造函数
```C++
// ①
thread() noexcept;
// ②
thread( thread&& other ) noexcept;
// ③
template< class Function, class... Args >
explicit thread( Function&& f, Args&&... args );
// ④
thread( const thread& ) = delete;
```
**构造函数①**：默认构造函，构造一个线程对象，在这个线程中不执行任何处理动作  
**构造函数②**：移动构造函数，将 other 的线程所有权转移给新的thread 对象。之后 other 不再表示执行线程。  
**构造函数③**：创建线程对象，并在该线程中执行函数f中的业务逻辑，args是要传递给函数f的参数
任务函数f的可选类型有很多，具体如下：
**普通函数，类成员函数，匿名函数，仿函数**（这些都是可调用对象类型）
可以是可调用对象包装器类型，也可以是使用绑定器绑定之后得到的类型（仿函数）  
**构造函数④**：使用=delete显示删除拷贝构造, 不允许线程对象之间的拷贝
***
**注**：当一个thread对象拥有可调用对象之后便立即开始在子线程中执行可调用对象的函数。如创建thread对象时使用第三种构造函数，或是使用operator=对临时对象做移动构造。如以下两句在创建对对象时便直接开始在子线程运行func函数。
```C++
std::thread th1(func,arg);
std::thread th2=std::thread(func,arg);
```
### 成员函数
```C++
std::thread::id get_id() const noexcept;
```
每个线程被创建出来都会有一个对应的id

```C++
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void func(int num, string str) {
    for (int i = 0; i < 10; ++i) {
        cout << "子线程: i = " << i << "num: " 
             << num << ", str: " << str << endl;
    }
}

void func1() {
    for (int i = 0; i < 10; ++i) {
        cout << "子线程: i = " << i << endl;
    }
}

int main() {
    cout << "主线程的线程ID: " << this_thread::get_id() << endl;
    thread t(func, 520, "i love you");
    thread t1(func1);
    cout << "线程t 的线程ID: " << t.get_id() << endl;
    cout << "线程t1的线程ID: " << t1.get_id() << endl;
}
```
***
```C++
void join();
```
主动等待线程运行结束（阻塞），在某个线程中使用子线程调用join函数，即的带该子线程运行结束后再运行当前线程接下来的语句。
***
```C++
void detach()
```
detach()的作用是将子线程和主线程的关联分离，也就是说detach()后子线程在后台独立继续运行，主线程无法再取得子线程的控制权，即使主线程结束，子线程未执行也不会结束。当主线程结束时，由运行时库负责清理与子线程相关的资源。实际应用如让一个文字处理应用同时编辑多个文档，让每个文档处理窗口拥有自己的线程，每个线程运行同样的代码，并隔离不同窗口处理的数据。
***
```c++
bool joinable() const noexcept；
```
返回值为true：主线程和子线程之间有关联（连接）关系  
返回值为false：主线程和子线程之间没有关联（连接）关系
```C++
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void foo() {
    this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    thread t;
    cout << "before starting, joinable: " << t.joinable() << endl; //没有关联 打印0

    t = thread(foo);
    cout << "after starting, joinable: " << t.joinable() << endl;  //有关联 打印1

    t.join();
    cout << "after joining, joinable: " << t.joinable() << endl;  //t执行完毕 没有关联

    thread t1(foo);
    cout << "after starting, joinable: " << t1.joinable() << endl;  //有关联
    t1.detach();
    cout << "after detaching, joinable: " << t1.joinable() << endl;  //分离 没有关联
}
```
***
```c++
// move (1)	
thread& operator= (thread&& other) noexcept;
// copy [deleted] (2)	
thread& operator= (const other&) = delete;
```
**注**：线程中的资源是不能被复制的，因此通过=操作符进行赋值操作最终并不会得到两个完全相同的对象。  
**通过以上=操作符的重载声明可以得知**：  
如果other是一个右值，会进行资源所有权的转移  
如果other不是右值，禁止拷贝，该函数被显示删除（=delete），不可用
***

## std::this_thread 命名空间
```c++
thread::id get_id() noexcept;
```
获取当前线程id的公共成员函数

```c++
template <class Rep, class Period>
  void sleep_for (const chrono::duration<Rep,Period>& rel_time);
```
***
让当前线程休眠一段时间 配合让std::chrono::seconds 使用
```c++
std::this_thread::sleep_for(std::chrono::seconds(1)); //休眠一秒
```
***
```c++
template <class Clock, class Duration>
  void sleep_until (const chrono::time_point<Clock,Duration>& abs_time);
```
让当前线程休眠到某一时刻
***
```c++
void yield() noexcept;
```
命名空间this_thread中提供了一个非常绅士的函数yield()，在线程中调用这个函数之后，处于运行态的线程会主动让出自己已经抢到的CPU时间片，最终变为就绪态，这样其它的线程就有更大的概率能够抢到CPU时间片了。使用这个函数的时候需要注意一点，线程调用了yield()之后会主动放弃CPU资源，但是这个变为就绪态的线程会马上参与到下一轮CPU的抢夺战中，不排除它能继续抢到CPU时间片的情况，这是概率问题。

***
# mutex头文件
## std::mutex
### 成员函数
```c++
void lock();
void unlock();
```
lock用于给临界区加锁，当一个线程运行这条语句后，该线程便抢到这个互斥锁的资源，其它有这个互斥锁lock语句运行到此句时便阻塞在此句。当拥有该互斥锁资源的线程运行unlock时便打开该锁，其他线程可以继续抢该锁的资源。
```c++
bool trylock();
```
当其他线程抢到互斥锁资源后，运行这条语句，会返回false执行其他语句，而不会阻塞在此处。当其他线程没有抢到该锁资源是，这条语句返回true并将互斥锁上锁。
***
## std::lock_guard
std::lock_guard 是一个模板类 ，可以简化mutex的操作，采用了RAII的技术，也可以避免忘记unlock造成的死锁。但是绘出是临界区过大降低运行效率。
```c++
// 类的定义，定义于头文件 <mutex>
template< class Mutex >
class lock_guard;
// 常用构造函数
explicit lock_guard( mutex_type& m );
```
```c++
std::mutex g_num_mutex;
void slow_increment(int id) {
    for (int i = 0; i < 3; ++i) {
        // 使用哨兵锁管理互斥锁
        std::lock_guard<mutex> lock(g_num_mutex);
        ++g_num;
        cout << id << " => " << g_num << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}
```
***
## std::recursive_mutex
递归互斥锁std::recursive_mutex允许同一线程多次获得互斥锁，可以用来解决同一线程需要多次获取互斥量时死锁的问题。  
虽然递归互斥锁可以解决同一个互斥锁频繁获取互斥锁资源的问题，但是还是建议少用，主要原因如下：  
1.使用递归互斥锁的场景往往都是可以简化的，使用递归互斥锁很容易放纵复杂逻辑的产生，从而导致bug的产生  
2.递归互斥锁比非递归互斥锁效率要低一些。  
3.递归互斥锁虽然允许同一个线程多次获得同一个互斥锁的所有权，但最大次数并未具体说明，一旦超过一定的次数，就会抛出std::system错误。
***
## std::timed_mutex
std::timed_mutex是超时独占互斥锁，主要是在获取互斥锁资源时增加了超时等待功能，因为不知道获取锁资源需要等待多长时间，为了保证不一直等待下去，设置了一个超时时长，超时后线程就可以解除阻塞去做其他事情了。
```c++
void lock();
bool try_lock();
void unlock();

// std::timed_mutex比std::_mutex多出的两个成员函数
template <class Rep, class Period>
  bool try_lock_for (const chrono::duration<Rep,Period>& rel_time);

template <class Clock, class Duration>
  bool try_lock_until (const chrono::time_point<Clock,Duration>& abs_time);
```
std::timed_mutex比std::mutex多了两个成员函数：try_lock_for()和try_lock_until()：  
try_lock_for函数是当线程获取不到互斥锁资源的时候，让线程阻塞一定的时间长度  
try_lock_until函数是当线程获取不到互斥锁资源的时候，让线程阻塞到某一个指定的时间点  
关于两个函数的返回值：当得到互斥锁的所有权之后，函数会马上解除阻塞，返回true，如果阻塞的时长用完或者到达指定的时间点之后，函数也会解除阻塞，返回false
***
## std::recursive_timed_mutex
带有超时的递归互斥锁

## std::call_once
在某些特定情况下，某些函数只能在多线程环境下调用一次，比如：要初始化某个对象，而这个对象只能被初始化一次，就可以使用std::call_once()来保证函数在多线程环境下只能被调用一次。使用call_once()的时候，需要一个once_flag作为call_once()的传入参数，该函数的原型如下：
```c++
template< class Callable, class... Args >
void call_once( std::once_flag& flag, Callable&& f, Args&&... args );
```
flag：once_flag类型的对象，要保证这个对象能够被多个线程同时访问到  
f：回调函数，可以传递一个有名函数地址，也可以指定一个匿名函数  
args：作为实参传递给回调函数  
```c++
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

once_flag g_flag;
void do_once(int a, string b) {
    cout << "name: " << b << ", age: " << a << endl;
}

void do_something(int age, string name) {
    static int num = 1;
    call_once(g_flag, do_once, 19, "luffy");
    cout << "do_something() function num = " << num++ << endl;
}

int main() {
    thread t1(do_something, 20, "ace");
    thread t2(do_something, 20, "sabo");
    thread t3(do_something, 19, "luffy");
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
```
***
# condition_variable 头文件
条件变量是C++11提供的另外一种用于等待的同步机制，它能阻塞一个或多个线程，直到收到另外一个线程发出的通知或者超时时，才会唤醒当前阻塞的线程。条件变量需要和互斥量配合起来使用，C++11提供了两种条件变量：
condition_variable：需要配合std::unique_lock进行wait操作，也就是阻塞线程的操作。  
condition_variable_any：可以和任意带有lock()、unlock()语义的mutex搭配使用，也就是说有四种：  
std::mutex：独占的非递归互斥锁  
std::timed_mutex：带超时的独占非递归互斥锁  
std::recursive_mutex：不带超时功能的递归互斥锁  
std::recursive_timed_mutex：带超时的递归互斥锁  
条件变量通常用于生产者和消费者模型，大致使用过程如下：  
拥有条件变量的线程获取互斥量  
循环检查某个条件，如果条件不满足阻塞当前线程，否则线程继续向下执行  
产品的数量达到上限，生产者阻塞，否则生产者一直生产。。。  
产品的数量为零，消费者阻塞，否则消费者一直消费。。。  
条件满足之后，可以调用notify_one()或者notify_all()唤醒一个或者所有被阻塞的线程  
由消费者唤醒被阻塞的生产者，生产者解除阻塞继续生产。。。
由生产者唤醒被阻塞的消费者，消费者解除阻塞继续消费。。。
## 成员函数
```c++
// ①
void wait (unique_lock<mutex>& lck);
// ②
template <class Predicate>
void wait (unique_lock<mutex>& lck, Predicate pred);
```
第一种形式只有一个参数unique_lock<mutex>&，调用wait时，若参数互斥量lck被锁定，则wait会阻塞，并将lck打开，让其他线程可以获取到该锁资源。当调用notify_all或notify_one函数时，wait阻塞解开，该线程会重新获得lck的资源。   
第二种形式除了unique_lock<mutex>&参数外，第二个参数pred，即函数指针。当函数运行到该wait()函数时，或者pred()返回值为false则wait阻塞。并将lck打开，让其他线程可以获取到该锁资源。当调用notify_all或notify_one函数时，wait阻塞解开，该线程会重新获得lck的资源。   

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>

class taskQueue
{
public:
    taskQueue(int size) : maxSize(size) {}

    void put(int num){
        std::unique_lock<std::mutex> locker(m_mutex);
        while (m_queue.size() == maxSize) {
            std::cout << "任务队列已满！！请等待后再生产！！ " << std::this_thread::get_id() << std::endl;
            full.wait(locker);
        }
        m_queue.push(num);
        std::cout << "生产线程:" << std::this_thread::get_id() << " num=" << num << " 已入队列！！" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        empty.notify_all();
    }

    void take() {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (!m_queue.size()) {
            std::cout << "任务队列为空！！请等待后再消费！！ " << std::this_thread::get_id() << std::endl;
            empty.wait(locker);
        }
        int num = m_queue.front();
        m_queue.pop();
        std::cout << "消费线程:" << std::this_thread::get_id() << " num=" << num << " 已出队列！！" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        full.notify_all();
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> guard(m_mutex);
        return !m_queue.size();
    }

    bool isFull() {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_queue.size() == maxSize;
    }

    int size() {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_queue.size();
    }

private:
    int maxSize;
    std::mutex m_mutex;
    std::condition_variable full;
    std::condition_variable empty;
    std::queue<int> m_queue;
};

int main() {
    taskQueue task(5);
    std::function<void(int)> producer = std::bind(&taskQueue::put, &task, std::placeholders::_1);
    std::function<void(void)> consumer = std::bind(&taskQueue::take, &task);
    std::thread thP[20], thC[20];
    for (int i = 0; i < 20; ++i) {
        thP[i] = std::thread(producer, i + 1000);
        thC[i] = std::thread(consumer);
    }
    for (int i = 0; i < 20; ++i) {
        thP[i].join();
        thC[i].join();
    }
    return 0;
}
```
***
# 信号量
信号量：首先是一个变量，其次是计数器。它是多线程环境下使用的一种设施，信号量在创建时需要设置一个初始值，表示同时可以有几个任务（线程）可以访问某一块共享资源。
一个任务要想访问共享资源，前提是信号量大于0，当该任务成功获得资源后，将信号量的值减 1；  
若当前信号量的值小于 0，表明无法获得信号量，该任务必须被挂起，等待信号量恢复为正值的那一刻；  
当任务执行完之后，必须释放信号量，对应操作就是信号量的值加 1。  
另外，对信号量的操作（加、减）都是原子的。互斥锁（Mutex）就是信号量初始值为 1 时的特殊情形，即同时只能有一个任务可以访问共享资源区。  

## pthread中的信号量
```c++
#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define MAX 5

pthread_mutex_t mutex;
sem_t semP;
sem_t semC;

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
        sem_wait(&semP);
        pthread_mutex_lock(&mutex);
        node* newNode=new node;
        newNode->number=rand()%1000;
        newNode->next=head;
        head=newNode;
        std::cout<<"生产者： "<<pthread_self()<<" 产品编号： "<<newNode->number<<" 商品存量: "<<node::getConut()<<std::endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&semC);
        sleep(rand()%5);
    }

    return NULL;
}

void* consumer(void* arg) {
 
    while(true) {
        sem_wait(&semC);
        pthread_mutex_lock(&mutex);
        node* hNode=head;
        std::cout<<"消费者： "<<pthread_self()<<" 产品编号： "<<hNode->number<<" 商品存量: "<<node::getConut()-1<<std::endl;
        head=hNode->next;
        delete hNode;
        pthread_mutex_unlock(&mutex);
        sem_post(&semP);
        sleep(rand()%5);
    }

    return NULL;
}

int main() {

    srand((unsigned)time(NULL));

    pthread_t tidP[5],tidC[5];

    pthread_mutex_init(&mutex,NULL);
    sem_init(&semP,0,10);
    sem_init(&semC,0,0);

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
    sem_destroy(&semP);
    sem_destroy(&semC);

    return 0;
}
```
信号量直到 C++20 才被支持，那 C++11 如何实现 semaphore 呢？答案是通过互斥锁和条件变量实现。
```c++
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::string FormatTimeNow(const char* format) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    char buf[20];
    std::strftime(buf, sizeof(buf), format, now_tm);
    return std::string(buf);
}

class Semaphore {
   public:
    explicit Semaphore(int count = 0) : count_(count) {}

    void Signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [=] { return count_ > 0; });
        --count_;
    }

   private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

Semaphore g_semaphore(4);
std::mutex g_io_mtx;

void DoWork() {
    g_semaphore.Wait();

    std::thread::id thread_id = std::this_thread::get_id();
    std::string now = FormatTimeNow("%H:%M:%S");
    {
        std::lock_guard<std::mutex> lock(g_io_mtx);
        std::cout << "Thread " << thread_id << ": wait succeeded"
                  << " (" << now << ")" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_semaphore.Signal();
}

int main() {
    int threadNum = 4;
    std::vector<std::thread> v;
    v.reserve(threadNum);
    for (std::size_t i = 0; i < threadNum; ++i) {
        v.emplace_back(&DoWork);
    }
    for (std::thread& t : v) {
        t.join();
    }
    return 0;
}
```
# 原子变量
C++11提供了一个原子类型std::atomic<T>，通过这个原子类型管理的内部变量就可以称之为原子变量，我们可以给原子类型指定bool、char、int、long、指针等类型作为模板参数（不支持浮点类型和复合类型）。  
原子指的是一系列不可被CPU上下文交换的机器指令，这些指令组合在一起就形成了原子操作。在多核CPU下，当某个CPU核心开始运行原子操作时，会先暂停其它CPU内核对内存的操作，以保证原子操作不会被其它CPU内核所干扰。  
由于原子操作是通过指令提供的支持，因此它的性能相比锁和消息传递会好很多。相比较于锁而言，原子类型不需要开发者处理加锁和释放锁的问题，同时支持修改，读取等操作，还具备较高的并发性能，几乎所有的语言都支持原子类型。  
可以看出原子类型是无锁类型，但是无锁不代表无需等待，因为原子类型内部使用了CAS循环，当大量的冲突发生时，该等待还是得等待！但是总归比锁要好。  
C++11内置了整形的原子变量，这样就可以更方便的使用原子变量了。在多线程操作中，使用原子变量之后就不需要再使用互斥量来保护该变量了，用起来更简洁。因为对原子变量进行的操作只能是一个原子操作（atomic operation），原子操作指的是不会被线程调度机制打断的操作，这种操作一旦开始，就一直运行到结束，中间不会有任何的上下文切换。多线程同时访问共享资源造成数据混乱的原因就是因为CPU的上下文切换导致的，使用原子变量解决了这个问题，因此互斥锁的使用也就不再需要了。
```c++
#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
using namespace std;

struct Counter {
    void increment() {
        for (int i = 0; i < 10; ++i) {
            m_value++;
            cout << "increment number: " << m_value
                << ", theadID: " << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }

    void decrement() {
        for (int i = 0; i < 10; ++i) {
            m_value--;
            cout << "decrement number: " << m_value
                << ", theadID: " << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
    // atomic<int> == atomic_int
    atomic_int m_value = 0;
};

int main() {
    Counter c;
    auto increment = bind(&Counter::increment, &c);
    auto decrement = bind(&Counter::decrement, &c);
    thread t1(increment);
    thread t2(decrement);

    t1.join();
    t2.join();

    return 0;
}
```
# future头文件
future是一个模板类
```c++
// 定义于头文件 <future>
template< class T > class future;
template< class T > class future<T&>;
template<>          class future<void>;
```
## 构造函数
```c++
// ①
future() noexcept;
// ②
future( future&& other ) noexcept;
// ③
future( const future& other ) = delete;
```
## operator=
```c++
future& operator=( future&& other ) noexcept;
future& operator=( const future& other ) = delete;
```
如果other是右值，那么转移资源的所有权  
如果other是非右值，不允许进行对象之间的拷贝（该函数被显示删除禁止使用）
