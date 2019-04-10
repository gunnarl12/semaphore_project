#include <iostream>
#include <string>
#include <iomanip>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>
#include <thread>
#include <pthread.h>
#include <sys/time.h>
#include <chrono>

//i semaphore with 4 units
using namespace std;


//double tellTime(double beginHere);

void customers(int cid)  {
        cout << "Customer id: " << cid << endl;
}
sem_t waitingChairs;

int main() {
        int serviceSeatCount;
        sem_init(&waitingChairs,0, 4);

        sem_getvalue(&waitingChairs, &serviceSeatCount);
        cout << "\nwaitingChair semaphore value: " << serviceSeatCount << endl;
        sem_wait(&waitingChairs);
        sem_getvalue(&waitingChairs, &serviceSeatCount);
        cout << "\nBefore post: waitingChair semaphore value: " << serviceSeatCount << endl;
        sem_wait(&waitingChairs);
        sem_post(&waitingChairs);
        sem_post(&waitingChairs);
        sem_getvalue(&waitingChairs, &serviceSeatCount);
        cout << "\nAfter post: waitingChair semaphore value: " << serviceSeatCount << endl;

        sem_destroy(&waitingChairs);

        return 0;
}

