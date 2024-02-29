#include <iostream>
#include <thread>
#include "skiplist.h"

#define NUM_THREADS 1
#define TEST_COUNT 1000000

Skiplist<int, std::string> skiplist(18);

void test() {
    std::cout << std::endl << "开始插入:" << std::endl;
    skiplist.insert_element(1, "Skiplist", true);
    skiplist.insert_element(4, "使用C++", true);
    skiplist.insert_element(5, "由Jacky Jiang编写", true);
    skiplist.insert_element(14, "这是一个简单的K-V跳表查询", true);
    skiplist.insert_element(19, "node.h实现单个节点的定义", true);
    skiplist.insert_element(19, "在自动测试中，一百万的插入和读取分别用时0.81s和0.79s", true);
    skiplist.insert_element(810, "skiplist.h实现整个跳表的定义", true);
    std::cout << "当前SkipList的大小:" << skiplist.size() << std::endl;
    std::cout << std::endl << "当前SkipList:" << std::endl;
    skiplist.display();
    std::cout << std::endl << "开始查询:" << std::endl;
    skiplist.search_element(5, true);
    skiplist.search_element(9, true);
    std::cout << std::endl << "开始移除:" << std::endl;
    skiplist.remove_element(14, true);
    skiplist.remove_element(81, true);
    std::cout << "当前SkipList的大小:" << skiplist.size() << std::endl;
    std::cout << std::endl << "程序结束" << std::endl;
}

void insert(int threadID) {
    std::cout << "当前线程ID: " << threadID << std::endl;
    int tem = TEST_COUNT / NUM_THREADS;
    for (int i = threadID * tem, count = 0; count < tem; i++) {
        count++;
        skiplist.insert_element(rand() % TEST_COUNT, "test");
    }
}

void get(int threadID) {
    std::cout << "当前线程ID: " << threadID << std::endl;
    int tem = TEST_COUNT / NUM_THREADS;
    for (int i = threadID * tem, count = 0; count < tem; i++) {
        count++;
        skiplist.search_element(rand() % TEST_COUNT);
    }
}

void stress_test() {
    std::thread threads[NUM_THREADS];
    int status;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        std::cout << "创建线程 " << i << std::endl;
        threads[i] = std::thread(insert, i);
    }

    for (auto &thread: threads)
        thread.join();

    auto finish_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(
            finish_time - start_time);
    std::cout << "插入测试用时(s): " << duration.count() << std::endl;

    std::cout << "--------------------" << std::endl;
    start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        std::cout << "创建线程 " << i << std::endl;
        threads[i] = std::thread(insert, i);
    }

    for (auto &thread: threads)
        thread.join();

    finish_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(finish_time - start_time);
    std::cout << "查询测试用时(s): " << duration.count() << std::endl;
}

int main() {
    std::cout << "1 = 测试" << std::endl
              << "2 = 压力测试" << std::endl;
    int choice = 0;
    try {
        std::cin >> choice;
    } catch (std::exception e) { ;
    }

    switch (choice) {
        case 1:
            test();
            break;
        case 2:
            stress_test();
            break;
        default:
            std::cout << "未知输入，已退出" << std::endl;
    }
    return 0;
}
