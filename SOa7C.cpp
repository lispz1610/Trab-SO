#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Encapsulamento completo do controle de concorrência
class Estacionamento {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int vagas_ocupadas = 0;
    const int capacidade_maxima;

public:
    // Construtor permite definir estacionamentos de qualquer tamanho
    Estacionamento(int capacidade) : capacidade_maxima(capacidade) {}

    void usar_vaga(int id) {
        // 1. Bloqueia para verificar a vaga
        std::unique_lock<std::mutex> lock(mtx);
        
        if (vagas_ocupadas == capacidade_maxima) {
            std::cout << "Carro " << id << " aguardando vaga...\n";
        }
        
        // Aguarda até ter vaga
        cv.wait(lock, [this] { return vagas_ocupadas < capacidade_maxima; });
        
        vagas_ocupadas++;
        std::cout << ">> Carro " << id << " ENTROU. Ocupacao: " << vagas_ocupadas << "/" << capacidade_maxima << "\n";
        
        // 2. Libera o lock temporariamente para o carro ficar estacionado (Seção Não-Crítica)
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        // 3. Bloqueia novamente para sair e atualizar o contador
        lock.lock();
        vagas_ocupadas--;
        std::cout << "<< Carro " << id << " SAIU. Ocupacao: " << vagas_ocupadas << "/" << capacidade_maxima << "\n";
        
        // Desbloqueia e avisa APENAS UM carro da fila (notify_one é mais eficiente que notify_all aqui)
        lock.unlock();
        cv.notify_one(); 
    }
};

int main() {    
    Estacionamento est(3); // Instancia o estacionamento com 3 vagas
    std::vector<std::thread> carros;

    for (int i = 1; i <= 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // Passamos o ID e a referência do estacionamento para a lambda
        carros.emplace_back([&est](int id) { est.usar_vaga(id); }, i);
    }

    for (auto& t : carros) t.join();

    std::cout << "Simulacao encerrada. Nenhuma vaga foi excedida!\n";
    return 0;
}
