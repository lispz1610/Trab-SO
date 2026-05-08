//Questão 6.5: Barbeiro sonolento - versão errada

#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <ctime> 
using namespace std;

#define CHAIRS 5 //cadeiras de espera

int waiting = 0; // clientes esperando
volatile bool barbeiroDormindo = false; 

void wakeup_barbeiro() {
    if (barbeiroDormindo) {
        barbeiroDormindo = false;
        cout << "Barbeiro acordou" << endl;
    }
}

void sleep_barbeiro() {
    barbeiroDormindo = true;
    while (barbeiroDormindo);
    cout << "Barbeiro foi acordado" << endl; //chegando outro cliente
}

void barbeiro() {
    while (true) {
        if (waiting == 0) {
            cout << "Sala vazia: barbeiro vai dormir" << endl;
            sleep_barbeiro(); //erro ocorre se cliente chegar e barbeiro estiver dormindo
        }

        waiting = waiting - 1; 
        cout << "[BARBEIRO] Cortando cabelo... ha " << waiting << " clientes esperando." << endl;
        
        this_thread::sleep_for(chrono::milliseconds(500)); //tempo para o corte
    }
}
void cliente(int id) {
    int arrive = rand() % 1000; //chegada de clientes aleatoria
    this_thread::sleep_for(chrono::milliseconds(arrive));

    if (waiting < CHAIRS) {
        waiting++;
        cout << "Cliente " << id << " chegou e sentou. Ha " << waiting << " clientes esperando." << endl;

        if (barbeiroDormindo) { //pode ocorrer sinal perdido aqui
            barbeiroDormindo = false; 
            cout << "Cliente" << id << " acordou o barbeiro" << endl;
        }
    } else {
        cout << "Cliente " << id << " viu que nao havia mais cadeiras e foi embora." << endl;
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