#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Estacionamento {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int vagas_ocupadas = 0;
    const int capacidade_maxima;

public:
    Estacionamento(int capacidade) : capacidade_maxima(capacidade) {}

    void usar_vaga(int id) {
        // 1. Bloqueia para verificar a vaga
        std::unique_lock<std::mutex> lock(mtx);
        
        if (vagas_ocupadas == capacidade_maxima) {
            std::cout << "Carro " << id << " aguardando vaga...\n";
        }
        
        cv.wait(lock, [this] { return vagas_ocupadas < capacidade_maxima; });
        
        vagas_ocupadas++;
        std::cout << ">> Carro " << id << " ENTROU. Ocupacao: " << vagas_ocupadas << "/" << capacidade_maxima << "\n";
        
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        lock.lock();
        vagas_ocupadas--;
        std::cout << "<< Carro " << id << " SAIU. Ocupacao: " << vagas_ocupadas << "/" << capacidade_maxima << "\n";

        lock.unlock();
        cv.notify_one(); 
    }
};

int main() {    
    Estacionamento est(3);
    std::vector<std::thread> carros;

    for (int i = 1; i <= 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        carros.emplace_back([&est](int id) { est.usar_vaga(id); }, i);
    }

    for (auto& t : carros) t.join();

    std::cout << "Simulacao encerrada. Nenhuma vaga foi excedida!\n";
    return 0;
}
