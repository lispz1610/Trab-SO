#include <iostream>
#include <semaphore.h>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

mutex acesso_leitores;                              //mutex para alterar a quantidade de leitores
sem_t acesso_dados;                                 //mutex pra controle do acesso ao banco de dados
int leitores = 0;                                   //quantidade de leitores

void ler_dados(int id){
    cout<<"Leitor "<<id<<" lendo dados."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //tempo aleatorio para ler dado
    cout<<"Leitor "<<id<<" saiu do BD."<<endl;
}
void escreve_dados(int id){
    cout<<"Escritor "<<id<<" escrevendo dados."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //tempo aleatorio para escrever dado
    cout<<"Escritor "<<id<<" saiu do BD."<<endl;
}

void leitor(int id){
    while(true){
        acesso_leitores.lock();                     //acessa a variavel leitores
        leitores++;                                 //entra pra lista de leitores
        if(leitores == 1){ 
            sem_wait(&acesso_dados);                    //se for o primeiro leitor, tenta acessar os dados
            cout<<"Dados trancados pelos leitores."<<endl;}
        acesso_leitores.unlock();                   //libera mutex
            ler_dados(id);                   
        acesso_leitores.lock();                     //acessa a variavel leitores
        leitores--;                                 //sai da lista de leitores
        if(leitores == 0){
            sem_post(&acesso_dados);                    //se for o último leitor, libera banco de dados
            cout<<"Dados liberados.\n"<<endl;}
        acesso_leitores.unlock();                   //libera mutex
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000)); //tempo pra usar os dados
    }
}

void escritor(int id){
    while(true){
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000)); //tempo pra gerar dados
        sem_wait(&acesso_dados);                    //tenta acessar dados
        cout<<"Dados trancados pelo escritor."<<endl;
        escreve_dados(id);                    
        sem_post(&acesso_dados);                    //libera dados
        cout<<"Dados liberados.\n"<<endl;
    }
}

int main(){
    srand(time(0));                                 //gerador de seed aleatória
    sem_init(&acesso_dados, 0, 1);                  //iniciar semaforo

    thread thread1(leitor, 0);                      //cria os leitores
    thread thread2(leitor, 1);
    thread thread3(leitor, 2);
    thread thread4(escritor, 3);                    //cria escritores
    thread thread5(escritor, 4);

    thread1.join();                                 //espera threads terminarem os serviços
    thread2.join();
    thread3.join();
    thread4.join();

    sem_destroy(&acesso_dados);                     //destroi semaforo
    return 0;
}