//Questão 6.5: Barbeiro Sonolento - versão correta 

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

#define CHAIRS 5

mutex mtx;   //  exclusão mútua
condition_variable cv_barbeiro;    
condition_variable cv_cliente;     
int waiting = 0;                   
bool barbeiro_ocupado = false;     

void barbeiro() {
    while (true) {
        unique_lock<mutex> lock(mtx);

        while (waiting == 0) {
            cout << "[BARBEIRO] Sala vazia. Dormindo." << endl;
            cv_barbeiro.wait(lock); 
        }

        waiting--;
        cout << "[BARBEIRO] Iniciando corte. Clientes na sala: " << waiting << endl;
        
        lock.unlock(); 

        this_thread::sleep_for(chrono::milliseconds(500)); 

        lock.lock(); 
        cout << "[BARBEIRO] Corte finalizado para o cliente atual!" << endl;
        
        cv_cliente.notify_one(); 
        
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(10)); 
    }
}

void cliente(int id) {
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));

    unique_lock<mutex> lock(mtx);
    if (waiting < CHAIRS) {
        waiting++;
        cout << "Cliente " << id << " entrou e sentou. (Cadeiras: " << waiting << ")" << endl;

        cv_barbeiro.notify_one(); 

        cv_cliente.wait(lock); 
        
        cout << "Cliente " << id << " agradece pelo cabelo cortado!" << endl;

    } else {
        cout << "Cliente " << id << " foi embora (lotado)." << endl;
    }
}

int main() {
   srand(time(0)); 
    
    thread barbeiro(barbeiro);
    barbeiro.detach(); 

    int id_atual = 1;

    while (true) {
        thread(cliente, id_atual).detach();
        id_atual++;
        int proxCliente = rand() % 2000; 
        this_thread::sleep_for(chrono::milliseconds(proxCliente));
        
        if (id_atual > 500) break; 
    }

    while(true) { this_thread::sleep_for(chrono::seconds(1)); } 

    return 0;
}