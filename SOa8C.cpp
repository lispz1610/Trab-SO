#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>

struct Trabalho {
    int id_processo;
    int paginas;
};

// Encapsulamento completo do Padrão Produtor-Consumidor
class SpoolerImpressao {
private:
    std::queue<Trabalho> fila;
    std::mutex mtx;
    std::condition_variable cv;
    bool ativa = true;

public:
    void enviar(int id_processo, int paginas) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            fila.push({id_processo, paginas});
            std::cout << "[SPOOLER] Processo " << id_processo << " enfileirou " << paginas << " paginas.\n";
        }
        cv.notify_one(); // Acorda a impressora, se ela estiver dormindo
    }

    void processar() {
        while (true) {
            Trabalho atual;
            {
                std::unique_lock<std::mutex> lock(mtx);
                
                cv.wait(lock, [this] { return !fila.empty() || !ativa; });

                if (!ativa && fila.empty()) break;

                atual = fila.front();
                fila.pop();
            }

            std::cout << "\n>> [IMPRESSORA] Imprimindo Processo " << atual.id_processo << "...\n";
            for (int i = 1; i <= atual.paginas; ++i) {
                std::cout << "   Pagina " << i << "/" << atual.paginas << "...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            std::cout << ">> [IMPRESSORA] Processo " << atual.id_processo << " concluido!\n";
        }
    }

    void desligar() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            ativa = false;
        }
        cv.notify_all();
    }
};

int main() {    
    SpoolerImpressao spooler;
    
    std::thread hardware([&spooler]() { spooler.processar(); });
    
    std::vector<std::thread> processos;

    for (int i = 1; i <= 10; ++i) {
        processos.emplace_back([&spooler](int id) { spooler.enviar(id, 2); }, i);
    }

    for (auto& t : processos) t.join();

    spooler.desligar();
    hardware.join();

    std::cout << "\nImpressora desligada com sucesso. Fila atendida na ordem correta!\n";
    return 0;
}
