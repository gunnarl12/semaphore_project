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

using namespace std;

// command to compile:
// g++ -o customer_service_system customer_service_system.cpp -lpthread -std=c++11

// set up semaphores
sem_t serviceChairs;
sem_t waitingChairs;

// project specification:
// waiting chairs 4
// service chairs 2
// assistants 2
// customers 12
//
//
// if no customers sitting on waiting chair, assistant goes to sleep
// if customer enters and all chairs are occupied (both waiting and service) customer leaves
// if assistants are busy but waiting chairs are available, customer sits in waiting chair
// if one or all assistants are asleep, customer wakes up one of the assistants to receive service
// all customers received in FCFC order
//
// cid	arrival_time service_time
// 1	3		15
// 2	7		10
// 3	8		8
// 4	9		5
// 5	11		12
// 6	12		4
// 7	14		8
// 8	16		14
// 9	19		7
// 10	22		2
// 11	34		9
// 12	39		3

// struct that ensures that each customer has the
// requisite qualities: id number, start time, service time,
// and a start watcher element that determines when a customer
// is first serviced by an assistant
struct t_args {
	int customerId;
	int customerStartTime;
	int customerTime;
	int runningTime;
	int startWatcher;
	int whileWatcher;
};

// global time keeper
double timer = 0;

// seating custumer function: determine whether a client is immediately seen, 
// waits before being seen, or leaves
void * seatCustomer(void * dummyParameter);

// start customer service function: determine when a client is seen
void * startCustomer(void * dummyParameter);

// "customer serviced" function: determine how long it takes to service a customer
// based on their individual needs 
void  customerServiced( void *dummyParameter);

int main() {

	// twelve structs for twelve threads initialized	
	struct t_args *threadArg1 = (struct t_args *)malloc(sizeof(struct t_args));
	struct t_args *threadArg2 = (struct t_args *)malloc(sizeof(struct t_args));
	struct t_args *threadArg3 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg4 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg5 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg6 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg7 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg8 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg9 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg10 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg11 = (struct t_args *)malloc(sizeof(struct t_args));
        struct t_args *threadArg12 = (struct t_args *)malloc(sizeof(struct t_args));


	// semaphores initialized
	sem_init(&serviceChairs, 0, 2);
	sem_init(&waitingChairs,0, 4);

	// threads initialized
	pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;

	// Customer 1 
	threadArg1->customerId = 1;
	threadArg1->customerStartTime = 3;
	threadArg1->customerTime = 16;
	threadArg1->runningTime = timer;
	threadArg1->startWatcher = 1;
	threadArg1->whileWatcher = 5;	

        // Customer 2 
	threadArg2->customerId = 2;
	threadArg2->customerStartTime = 7;
	threadArg2->customerTime = 11; 
        threadArg2->runningTime = timer;
	threadArg2->startWatcher = 1;
	
	// Customer 3 	
        threadArg3->customerId = 3;
	threadArg3->customerStartTime = 8;
        threadArg3->customerTime = 8;
        threadArg3->runningTime = 8;
	threadArg3->startWatcher = 1;

	// Customer 4
        threadArg4->customerId = 4;
        threadArg4->customerStartTime = 9;
        threadArg4->customerTime = 5;
        threadArg4->runningTime = 8;
        threadArg4->startWatcher = 1;

	// Customer 5
        threadArg5->customerId = 5;
        threadArg5->customerStartTime = 11;
        threadArg5->customerTime = 12;
        threadArg5->runningTime = 12;
        threadArg5->startWatcher = 1;

	// Customer 6
        threadArg6->customerId = 6;
        threadArg6->customerStartTime = 12;
        threadArg6->customerTime = 4;
        threadArg6->runningTime = 4;
        threadArg6->startWatcher = 1;

        // Customer 7
	threadArg7->customerId = 7;
        threadArg7->customerStartTime = 14;
        threadArg7->customerTime = 8;
        threadArg7->runningTime = 8;
        threadArg7->startWatcher = 1;

	// Customer 8
        threadArg8->customerId = 8;
        threadArg8->customerStartTime = 16;
        threadArg8->customerTime = 14;
        threadArg8->runningTime = 14;
        threadArg8->startWatcher = 1;

	// Customer 9
        threadArg9->customerId = 9;
        threadArg9->customerStartTime = 19;
        threadArg9->customerTime = 7;
        threadArg9->runningTime = 7;
        threadArg9->startWatcher = 1;

	// Customer 10
        threadArg10->customerId = 10;
        threadArg10->customerStartTime = 22;
        threadArg10->customerTime = 2;
        threadArg10->runningTime = 2;
        threadArg10->startWatcher = 1;

	// Customer 11
        threadArg11->customerId = 11;
        threadArg11->customerStartTime = 34;
        threadArg11->customerTime = 9;
        threadArg11->runningTime = 9;
        threadArg11->startWatcher = 1;

        // Customer 12
	threadArg12->customerId = 12;
        threadArg12->customerStartTime = 39;
        threadArg12->customerTime = 3;
        threadArg12->runningTime = 3;
        threadArg12->startWatcher = 1;

	// get time from system
	auto time1 = chrono::system_clock::now();
	
	// loop for 45 seconds
	while (timer < 46) {
		// sleep system for 1 second
		usleep(1000*1000);
		// get time from system
		auto time2 = chrono::system_clock::now();
		// get difference between initial time and updated time
		auto timePassed = (chrono::duration_cast<chrono::seconds>(time2-time1)).count();
		
		// update timer with new time that's passed
		timer = timePassed;
		
		// determine if Customer's start time has come.
		// if yes, then the thread is created and the Customer "arrives"
		if (timePassed == threadArg1->customerStartTime) {
			pthread_create(&t1, NULL, seatCustomer, (void*)threadArg1);
		}
		if (timePassed == threadArg2->customerStartTime) {
                        pthread_create(&t2, NULL, seatCustomer, (void*)threadArg2);
		}
		if (timePassed == threadArg3->customerStartTime) {
                        pthread_create(&t3, NULL, seatCustomer, (void*)threadArg3);
                }
                if (timePassed == threadArg4->customerStartTime) {
                        pthread_create(&t4, NULL, seatCustomer, (void*)threadArg4);
                }
                if (timePassed == threadArg5->customerStartTime) {
                        pthread_create(&t5, NULL, seatCustomer, (void*)threadArg5);
                }
                if (timePassed == threadArg6->customerStartTime) {
                        pthread_create(&t6, NULL, seatCustomer, (void*)threadArg6);
                }
                if (timePassed == threadArg7->customerStartTime) {
                        pthread_create(&t7, NULL, seatCustomer, (void*)threadArg7);
                }
                if (timePassed == threadArg8->customerStartTime) {
                        pthread_create(&t8, NULL, seatCustomer, (void*)threadArg8);
                }
                if (timePassed == threadArg9->customerStartTime) {
                        pthread_create(&t9, NULL, seatCustomer, (void*)threadArg9);
                }
                if (timePassed == threadArg10->customerStartTime) {
                        pthread_create(&t10, NULL, seatCustomer, (void*)threadArg10);
                }
                if (timePassed == threadArg11->customerStartTime) {
                        pthread_create(&t11, NULL, seatCustomer, (void*)threadArg11);
                }
                if (timePassed == threadArg12->customerStartTime) {
                        pthread_create(&t12, NULL, seatCustomer, (void*)threadArg12);
                }
	}

	// join all threads 
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);
        pthread_join(t6, NULL);
        pthread_join(t7, NULL);
        pthread_join(t8, NULL);
        pthread_join(t9, NULL);
        pthread_join(t10, NULL);
        pthread_join(t11, NULL);
        pthread_join(t12, NULL);


	// destroy semaphores after all operations have completed
	sem_destroy(&serviceChairs);
	sem_destroy(&waitingChairs);
	return 0;
}


void * seatCustomer(void *dummyParameter) {
		int checkService;
		int checkWaiting;
		cout <<"Time: " << timer << " Customer " << ((struct t_args*)dummyParameter)->customerId << " arrives" << endl;
		sem_getvalue(&waitingChairs, &checkWaiting);
                sem_getvalue(&serviceChairs, &checkService);
		if (checkService >= 1 ) {
			sem_wait(&serviceChairs);		// service chairs are available, take one away
			sem_getvalue(&serviceChairs, &checkService);	// update checkService variable
			startCustomer((void*)dummyParameter);		// proceed to startCustomer function
		} else if (checkService < 1 && checkWaiting >= 1 ) {
			sem_wait(&waitingChairs);		// decrement waitingChairs if service chairs are all taken
                        sem_getvalue(&waitingChairs, &checkWaiting); // update checkWaiting variable
			sem_wait(&serviceChairs);		// if customer is put in waiting chair, they are also put in service queue
                        startCustomer((void*)dummyParameter);	// after serviceChair, becomes available, proceed to startCustomer function
		} else if (checkWaiting == 0) {			// if no waiting chairs are available, leave
			cout << "Time: " << timer << " Customer " << ((struct t_args*)dummyParameter)->customerId << " leaves" << endl;
		}	

}
void * startCustomer(void * dummyParameter) {
		// use to determine whether or not a customer should considered serviced
		int checkService;
		int checkWaiting;
		// assign current semaphore unit count to seat checking variable
		sem_getvalue(&waitingChairs, &checkWaiting);
                sem_getvalue(&serviceChairs, &checkService);
		// print out customer start time
		cout << "Time: " << timer << " Customer " << ((struct t_args*)dummyParameter)->customerId << " starts" << endl;
		// reduce customer start watcher to 0
		// this attribute ensures that the customer's start time has passed
		((struct t_args*)dummyParameter)->startWatcher--;	
		// take the customer to the servicing section and determine when their service ends
		customerServiced( (void *) dummyParameter);
		// open up a waiting seat	
		sem_post(&waitingChairs);
}

void  customerServiced( void *dummyParameter) {
	int checkService;
	int checkWaiting;
	usleep((((struct t_args*)dummyParameter)->customerTime)*1000*1000);
	cout <<"Time: " << timer << " Customer " << ((struct t_args*)dummyParameter)->customerId << " done "<< endl;
	((struct t_args*)dummyParameter)->startWatcher++;
	sem_post(&serviceChairs);
}

