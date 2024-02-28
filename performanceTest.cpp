//
// Created by Fisher on 2024/2/28.
//

#include <iostream>
#include <chrono>

int max_level = 5;

// 你要测试的函数
int testFunction1() {
    int result = 0;
    srand(time(nullptr));
    while (rand() % 2) {
        result++;
        if (result == max_level)
            break;
    }
    return result;
}

int testFunction2() {
    int result = 0;
    srand(time(nullptr));
    while (rand() % 2) {
        result++;
    }
    result = (result < max_level) ? result : max_level;
    return result;
}

int main() {
    // 获取开始时间
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    for (int x = 0; x < 10; x++) {
        start = std::chrono::high_resolution_clock::now();
        // 执行函数一百万次
        for (int i = 0; i < 10000000; ++i) {
            testFunction1();
        }
        // 获取结束时间
        end = std::chrono::high_resolution_clock::now();
        // 计算并输出用时（以毫秒为单位）
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Execution time 1: " << duration.count() << " ms" << std::endl;

// ----------------------------------------------------------

        start = std::chrono::high_resolution_clock::now();

        // 执行函数一百万次
        for (int i = 0; i < 10000000; ++i) {
            testFunction2();
        }

        // 获取结束时间
        end = std::chrono::high_resolution_clock::now();

        // 计算并输出用时（以毫秒为单位）
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Execution time 2: " << duration.count() << " ms" << std::endl;
    }
    return 0;
}