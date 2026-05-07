#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

enum Direcao { NENHUMA = 0, ESQ_DIR = 1, DIR_ESQ = 2 };

class Ponte {
private:
    std::mutex mtx;
    std::condition_variable cv;
    Direcao direcao_atual = NENHUMA;
    int carros_na_ponte = 0;
    int passagens_consecutivas = 0;
    const int LIMITE_CONSECUTIVOS = 3;
    
    int esperando[3] = {0, 0, 0}; 

public:
    void atravessar(int id, Direcao dir, const char* nome_dir) {
        std::unique_lock<std::mutex> lock(mtx);
        esperando[dir]++;

        cv.wait(lock, [&]() {
            if (carros_na_ponte == 0) return true;
            if (direcao_atual != dir) return false;
            
            // Lógica de inanição simplificada
            Direcao oposta = (dir == ESQ_DIR) ? DIR_ESQ : ESQ_DIR;
            if (esperando[oposta] > 0 && passagens_consecutivas >= LIMITE_CONSECUTIVOS) return false;
            
            return true;
        });

        esperando[dir]--;
        if (direcao_atual != dir) {
            direcao_atual = dir;
            passagens_consecutivas = 0;
        }
        carros_na_ponte++;
        passagens_consecutivas++;
        std::cout << ">> Carro " << id << " (" << nome_dir << ") ENTROU. (Na ponte: " << carros_na_ponte << ")\n";

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        lock.lock();
        carros_na_ponte--;
        std::cout << "<< Carro " << id << " (" << nome_dir << ") SAIU.   (Na ponte: " << carros_na_ponte << ")\n";

        if (carros_na_ponte == 0) direcao_atual = NENHUMA;
        
        lock.unlock();
        cv.notify_all();
    }
};

int main() {
    std::cout << "--- Iniciando simulacao CORRIGIDA (Orientada a Objetos) ---\n";
    
    Ponte ponte;
    std::vector<std::thread> frota;

    auto dirigir = [&](int id, Direcao dir, const char* nome) {
        ponte.atravessar(id, dir, nome);
    };

    for (int i = 1; i <= 6; ++i) frota.emplace_back(dirigir, i, ESQ_DIR, "ESQ->DIR");
    for (int i = 7; i <= 12; ++i) frota.emplace_back(dirigir, i + 6, DIR_ESQ, "DIR->ESQ");

    for (auto& t : frota) t.join();

    std::cout << "Simulacao finalizada.\n";
    return 0;
}
