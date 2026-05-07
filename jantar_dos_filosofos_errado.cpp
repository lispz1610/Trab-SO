#include <iostream>
#include <semaphore.h>
#include <mutex>
#include <thread>
#include <chrono>
#define N 5

using namespace std;

class Mesa{                                             //cria uma mesa pra administrar os garfos
    private:
        mutex mtx;
        bool garfos[N];

    public:
        Mesa(){
            for(int i = 0; i < N; i++){
                garfos[i] = true;                       //comeca com todos os garfos na mesa
            }
        }
        bool dar_garfo(int i){                          
            mtx.lock();
            if(garfos[i]){
                garfos[i] = false;                      //mesa define que o garfo não está mais nela
                mtx.unlock();
                return true;                            //retorna liberação pro garfo ser pego
            }
            else{
                mtx.unlock();
                return false;                           //se o garfo não estiver disponível, sinaliza com false
            }
        }

        void repor_garfo(int i){
            mtx.lock();
            garfos[i] = true;                           //se o garfo for devolvido, repoe ele na mesa
            mtx.unlock();
        }
};

void pensar(int id){
    cout<<"Filosofo "<<id<<" esta pensando..."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //tempo para pensar
}

void pegar_garfo(int* n_garfos, int id_filosofo, int i, Mesa* mesa){
    if(*n_garfos >= 0 || *n_garfos < 2){                            //se tiver 1 ou nenhum garfo
        while (!mesa->dar_garfo(i)){                                //enquanto a mesa sinalizar que nao pode usar o garfo
            cout<<"Filosofo "<<id_filosofo<<" aguardando garfo..."<<endl;
            this_thread::sleep_for(chrono::milliseconds(1000));     //espera
        }
    }
    (*n_garfos)++;                                                  //quando o garfo for liberado, ganha um garfo
}

void soltar_garfo(int i, Mesa* mesa){
    mesa->repor_garfo(i);                                           //devolve o garfo pra mesa
}

void comer(int id){
    cout<<"Filosofo "<<id<<" esta comendo..."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //espera terminar de comer
}

void filosofo(int id, Mesa* mesa){
    while(true){
        int num_garfos = 0;                                         //comeca com 0 garfos
        pensar(id);
        pegar_garfo(&num_garfos, id, id, mesa);                     //tenta pegar garfo a direita
        pegar_garfo(&num_garfos, id, (id+N-1) % N, mesa);           //tenta pegar garfo a esquerda
        comer(id);
        soltar_garfo(id, mesa);                                     //devolve garfo a direita
        soltar_garfo((id+N-1) % N, mesa);                           //devolve garfo a esquerda
    }
}

int main(){
    srand(time(0));                                                 //seed aleatoria pro rand

    Mesa mesa;                                                      //inicia a mesa

    thread t1(filosofo, 0, &mesa);                                  //inicia os filosofos
    thread t2(filosofo, 1, &mesa);
    thread t3(filosofo, 2, &mesa);
    thread t4(filosofo, 3, &mesa);
    thread t5(filosofo, 4, &mesa);

    t1.join();                                                      //espera filosofo terminar operacao
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}