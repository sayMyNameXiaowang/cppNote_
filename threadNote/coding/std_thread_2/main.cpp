#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>

std::mutex mutex;
int cur=0;

void func1(int count) {
    for(int i=0;i<count;++i) {
        mutex.lock();
        cur++;
        std::cout<<"子线程："<<std::this_thread::get_id()<<" cur:"<<cur<<std::endl;
        mutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

std::timed_mutex g_mutex;

// try_lock_for函数是当线程获取不到互斥锁资源的时候，让线程阻塞一定的时间长度
// try_lock_until函数是当线程获取不到互斥锁资源的时候，让线程阻塞到某一个指定的时间点
// 关于两个函数的返回值：当得到互斥锁的所有权之后，函数会马上解除阻塞，返回true，如果阻塞的时长用完或者到达指定的时间点之后，函数也会解除阻塞，返回false

void work() {
    std::chrono::seconds timeout(1);
    while (true) {
        // 通过阻塞一定的时长来争取得到互斥锁所有权
        if (g_mutex.try_lock_for(timeout)) {
            std::cout << "当前线程ID: " << std::this_thread::get_id() 
                << ", 得到互斥锁所有权..." << std::endl;
            // 模拟处理任务用了一定的时长
            std::this_thread::sleep_for(std::chrono::seconds(10));
            // 互斥锁解锁
            g_mutex.unlock();
            //break;
        }
        else {
            std::cout << "当前线程ID: " << std::this_thread::get_id() 
                << ", 没有得到互斥锁所有权..." << std::endl;
            // 模拟处理其他任务用了一定的时长
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

int main() {
    // std::thread th1(func1,30);
    // std::thread th2(func1,50);
    std::thread th1(work);
    std::thread th2(work);
    th1.join();
    th2.join();
    return 0;
}