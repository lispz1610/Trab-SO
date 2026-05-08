//Questão 6.2 - Produtor-consumidor com buffer limitado
//usando semáforos (versão correta)

#include <mutex>
#include <condition_variable>
#include <iostream> 
#include <thread>
using namespace std;
#define N 10
int buffer[N];
int in = 0;
int out = 0;

class Semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    Semaphore(int count_ = 0) : count(count_) {}

    void down() { 
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0) {
            cv.wait(lock);
        }
        count--;
    }

    void up() { 
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

Semaphore mtx(1); //mutex
Semaphore ept(N); //empty
Semaphore full(0);

void produtor() {
    while (true) {
        int item = (rand() % 100) + 1;

        ept.down(); // decresce empty
        mtx.down(); // entra na região critica
        //insere item
        buffer[in] = item;
        in = (in + 1) % N;
        cout << "[PRODUTOR] Colocou: " << item << endl;

        mtx.up();   // saio da região crítica
        full.up();  //incrementa contador de lugares preenchidos
    }
}

void consumidor() {
    while (true) {
        full.down();  //decresce full
        mtx.down(); //entra na região critica

        //removeItem(); //pega item do buffer
        int item = buffer[out];
        out = (out + 1) % N;
        cout << "[CONSUMIDOR] Pegou: " << item << endl;

        mtx.up();   // sai da região crítica
        ept.up();   // incrementa contador de lugares vazios
    }
}

int main(){
    thread t1(produtor);
    thread t2(consumidor);


    t1.join();
    t2.join();
    return 0;
}