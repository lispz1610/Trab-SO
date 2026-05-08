#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

int main() {
    int fila_trabalhos[100];
    int qtd_fila = 0;
    std::vector<std::thread> processos;

    auto enviar_impressao = [&](int id_processo) {
        int pos = qtd_fila;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
        
        fila_trabalhos[pos] = id_processo;
        qtd_fila = pos + 1;
        
        std::cout << "[ERRO] Processo " << id_processo << " colocou o trabalho na posicao " << pos << " da fila.\n";
    };

    for (int i = 1; i <= 10; ++i) {
        processos.emplace_back(enviar_impressao, i);
    }

    for (auto& t : processos) t.join();

    std::cout << "\nTotal de trabalhos registrados na fila: " << qtd_fila << " (deveria ser 10)\n";

    return 0;
}
