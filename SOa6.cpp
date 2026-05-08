#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

int main() {
    std::vector<std::thread> carros;

    auto atravessar_ponte = [](int id, const char* direcao) {
        std::cout << "[ERRO] Carro " << id << " (" << direcao << ") ENTRANDO na ponte.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        std::cout << "[ERRO] Carro " << id << " (" << direcao << ") SAINDO da ponte.\n";
    };

    for (int i = 1; i <= 6; ++i) {
        carros.emplace_back(atravessar_ponte, i, "ESQ -> DIR");
        carros.emplace_back(atravessar_ponte, i + 6, "DIR -> ESQ");
    }

    for (auto& t : carros) t.join();

    std::cout << "Simulacao finalizada.\n";
    return 0;
}
