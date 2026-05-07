#include <iostream>
#include <thread>
#include <vector>

int main() {
    const int ITERACOES = 1'000'000;
    int contador = 0;

    std::thread t1([&]() {
        for (int i = 0; i < ITERACOES; ++i) contador++;
    });

    std::thread t2([&]() {
        for (int i = 0; i < ITERACOES; ++i) contador--;
    });

    t1.join();
    t2.join();

    std::cout << "Valor final: " << contador;

    return 0;
}
