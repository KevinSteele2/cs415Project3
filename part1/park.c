#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    int number;
    int exploring_time;
} PArgs;

typedef struct{
    int wait_period;
    int ride_duration;
} CArgs;

int ticket_queue[1];
int ride_queue[1];
int tqueue_size = 0;
int rqueue_size = 0;

void* passenger_thread(void* arg){
    PArgs* p = (PArgs*)arg;
    printf("Passenger %d entered the park. \n", p->number);
    printf("Passenger %d is exploring the park...\n", p->number);
    sleep(p->exploring_time);
    printf("Passenger %d finished exploring, heading to ticket booth.\n", p->number);
    ticket_queue[tqueue_size] = p->number;
    tqueue_size++;
    // Possible print who is waiting in ticket queue here..
    printf("Passenger %d waiting in ticket queue\n", p->number);
    printf("Passenger %d acquired a ticket\n", p->number);
    int queued_passenger = ticket_queue[tqueue_size - 1];
    tqueue_size--;
    ride_queue[rqueue_size] = queued_passenger;
    rqueue_size++;
    // Possibly print ride queue here...
    printf("Passenger %d joined the ride queue\n", p->number);
    pthread_exit(NULL);
}

void* car_thread(void* arg){
    CArgs* c = (CArgs*)arg;
    printf("Car invoked load(), passengers loading...\n");
    sleep(c->wait_period);
    if(rqueue_size > 0){
        int p_boarding = ride_queue[rqueue_size - 1];
        rqueue_size--;
        printf("Passenger %d boarded car\n", p_boarding);
    }
    else{
        printf("No passengers in ride queue\n");
    }
    sleep(c->ride_duration);
    printf("Car has finished ride, passengers unloading...\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    int opt;
    int n = 1;
    int num_cars = 1;
    int capacity = 1;
    int wait_period = 3;
    int ride_duration = 3;
    int exploring_time = 5;

    while((opt = getopt(argc, argv, ":n:c:p:w:r:h")) != -1){ //get option from the getopt() method
        switch(opt){
            case 'n':
                n = atoi(optarg);
                break;
            case 'c':
                num_cars = atoi(optarg);
                break;
            case 'p':
                capacity = atoi(optarg);
                break;
            case 'w':
                wait_period = atoi(optarg);
                break;
            case 'r':
                ride_duration = atoi(optarg);
                break;
            case 'h':
                printf("How to use:\n");
                printf("To run, use ./park (parameters)\n");
                printf("What are the parameters?\n");
                printf("-n (int): the number of passenger threads\n");
                printf("-c (int): the number of cars\n");
                printf("-p (int): the capacity per car\n");
                printf("-w (int): the car waiting period in seconds\n");
                printf("-r (int): the ride duration in seconds\n");
                printf("-h: displays help message\n");
                exit(0);
            case ':':
                printf("option needs a value\n");
                break;
            case '?': 
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    for(; optind < argc; optind++){ //when some extra arguments are passed
        printf("Given extra arguments: %s\n", argv[optind]);
    }

    printf("===Duck Park Simulation===\n");
    printf("Simulation started with these parameters:\n");
    printf("- Number of passenger threads: %d\n", n);
    printf("- Number of cars: %d\n", num_cars);
    printf("- Capacity per car: %d\n", capacity);
    printf("- Park exploring time: %d\n", exploring_time);
    printf("- Car waiting period: %d\n", wait_period);
    printf("- Ride duration: %d\n", ride_duration);

    pthread_t passenger, car;
    PArgs pargs = {.number = 1, .exploring_time = exploring_time};
    CArgs cargs = {.wait_period = wait_period, .ride_duration = ride_duration};
    pthread_create(&passenger, NULL, passenger_thread, &pargs);
    pthread_create(&car, NULL, car_thread, &cargs);
    pthread_join(passenger, NULL);
    pthread_join(car, NULL);

    printf("Simulation Finished\n");
    return 0;
    
}