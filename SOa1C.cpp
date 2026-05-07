#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

int main() {
    const int ITERACOES = 1'000'000;
    
    std::atomic<int> contador{0};

    std::thread t1([&]() {
        for (int i = 0; i < ITERACOES; ++i) contador++;
    });

    std::thread t2([&]() {
        for (int i = 0; i < ITERACOES; ++i) contador--;
    });

    t1.join();
    t2.join();

    std::cout << "Valor final: ";

    return 0;
}
