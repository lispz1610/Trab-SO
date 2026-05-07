#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int leitores = 0;                                   //quantidade de leitores

void ler_dados(int id){
    cout<<"Leitor "<<id<<" entrou no BD/lendo dados."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //tempo aleatorio para ler dado
    cout<<"Leitor "<<id<<" saiu do BD."<<endl;
}
void escreve_dados(int id){
    cout<<"Escritor "<<id<<" entrou no BD/escrevendo dados."<<endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));    //tempo aleatorio para escrever dado
    cout<<"Escritor "<<id<<" saiu do BD."<<endl;
}

void leitor(int id){
    while(true){
        leitores++;                                 //entra pra lista de leitores
        ler_dados(id);                   
        leitores--;                                 //sai da lista de leitores
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000)); //tempo pra usar os dados
    }
}

void escritor(int id){
    while(true){
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000)); //tempo pra gerar dados
        escreve_dados(id);                    
    }
}

int main(){
    srand(time(0));                                 //gerador de seed aleatória

    thread thread1(leitor, 0);                      //cria os leitores
    thread thread2(leitor, 1);
    thread thread3(leitor, 2);
    thread thread4(escritor, 3);                    //cria escritores
    thread thread5(escritor, 4);

    thread1.join();                                 //espera threads terminarem os serviços
    thread2.join();
    thread3.join();
    thread4.join();

    return 0;
}