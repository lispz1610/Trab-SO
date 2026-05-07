#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

int main() {    
    const int CAPACIDADE_MAXIMA = 3;
    int vagas_ocupadas = 0;
    std::vector<std::thread> carros;

    auto tentar_estacionar = [&](int id) {
        std::cout << "Carro " << id << " chegou na cancela.\n";

        if (vagas_ocupadas < CAPACIDADE_MAXIMA) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            
            vagas_ocupadas++;
            std::cout << "[ERRO] Carro " << id << " ENTROU. Ocupacao: " << vagas_ocupadas << "/" << CAPACIDADE_MAXIMA << "\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            vagas_ocupadas--;
            std::cout << "Carro " << id << " SAIU. Ocupacao: " << vagas_ocupadas << "/" << CAPACIDADE_MAXIMA << "\n";
        } else {
            std::cout << "[RECUSADO] Carro " << id << " foi embora. Estacionamento lotado!\n";
        }
    };

    for (int i = 1; i <= 10; ++i) {
        carros.emplace_back(tentar_estacionar, i);
    }

    for (auto& t : carros) t.join();

    return 0;
}
