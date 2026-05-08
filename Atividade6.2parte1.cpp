

//Questão 6.2: Produtor-consumidor com buffer limitado
//versão sem semáforos (resultando em erros) 

#include <iostream>
#include <cstdlib> 
#include <thread>
#define N 10000 //qtde de lugares no buffer
int cont = 0; //lugares ocupados no buffer
int buffer[N];
int in = 0; //posição para inserção do item
int out = 0; //posição para remover item
volatile bool prodDormir = false;
volatile bool consumDormir = false;

using namespace std;
thread::id id_produtor;
thread::id id_consumidor;
thread::id id_consumidor2;
thread::id id_produtor2;
void wakeup(string id)
{
    if(id == "produtor"){ 
        prodDormir = false;
        cout << "produtor acordou " << endl;
    }
    if(id == "consumidor"){
         consumDormir = false;
         cout << "consumidor acordou " << endl;
    }
}


void sleep()
{
    auto id = std::this_thread::get_id();
    if(id == id_produtor){
        prodDormir = true;
        while(prodDormir)cout<<"Produtor dormiu "<<endl;
    }
    else if(id == id_consumidor){
        consumDormir = true;
        while (consumDormir)cout<<"Consumidor dormiu "<<endl;;
        
    }
}

void insereItem(int item){
    buffer[in] = item;
    in = (in + 1) % N;
    cout << "[PRODUTOR] Colocou: " << item << endl;
}
void removeItem()
{
    int item = buffer[out];
    out = (out + 1) % N;
    cout << "[CONSUMIDOR] Pegou: " << item << endl;
}
void produtor()
{ //quer colocar itens no buffer
    int item;
    while(true){
        item = (rand() % 100) + 1; //gera item
        if(cont == N) sleep(); 
        insereItem(item);
        cont = cont + 1;
        //cout << "produtor tem : "<< cont << " itens" << endl; //debugger
        if(cont == 1) wakeup("consumidor");
        
    }
}

void consumidor()
{ // quer retirar itens do buffer
    int item;
    while(true){
        if(cont == 0) sleep();
        removeItem();
        cont = cont - 1;
        //cout << "Ha  " << cont << " itens a serem consumidos " << endl;
        if(cont == N-1) wakeup("produtor");
        
    }
}


int main(){
    thread t1(produtor);
    thread t2(consumidor);
    thread t3(produtor);
    thread t4(consumidor);
    id_produtor = t1.get_id();
    id_consumidor = t2.get_id();
    id_produtor2 = t3.get_id();
    id_consumidor2 = t4.get_id();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}