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
    int capacity;
    int car_number;
} CArgs;

int *ticket_queue;
int *ride_queue;
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
    while(1){
        int on_ride = 0;
        printf("Car %d invoked load(), passengers loading...\n", c->car_number);
        sleep(c->wait_period);
        while(rqueue_size > 0 && on_ride < c->capacity){
            int p_boarding = ride_queue[rqueue_size - 1];
            rqueue_size--;
            printf("Passenger %d boarded car %d\n", p_boarding, c->car_number);
            on_ride++;
        }
        if(on_ride == 0){
            printf("No passengers in ride queue\n");
            break;
        }
        sleep(c->ride_duration);
        printf("Car %d has finished ride, passengers unloading...\n", c->car_number);
    }
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
    printf("- Park exploring time: random value between 1 and 5");
    printf("- Car waiting period: %d\n", wait_period);
    printf("- Ride duration: %d\n", ride_duration);

    ticket_queue = malloc(n * sizeof(int));
    ride_queue = malloc(n* sizeof(int));

    int i;
    pthread_t *passengers = malloc(n * sizeof(pthread_t));
    PArgs *all_pargs = malloc(n * sizeof(PArgs));
    for (i=0; i < n; i++){
        all_pargs[i].number = i+1;
        all_pargs[i].exploring_time = (rand()%5) + 1;
        pthread_create(&passengers[i], NULL, passenger_thread, &all_pargs[i]);
    }
    for (i = 0; i < n; i++){
        pthread_join(passengers[i], NULL);
    }

    pthread_t *cars = malloc(num_cars * sizeof(pthread_t));
    CArgs *all_cars = malloc(num_cars * sizeof(CArgs));
    for (i = 0; i< num_cars; i++){
        all_cars[i].wait_period = wait_period;
        all_cars[i].ride_duration = ride_duration;
        all_cars[i].capacity = capacity;
        all_cars[i].car_number = i+1;
        pthread_create(&cars[i], NULL, car_thread, &all_cars[i]);
    }
    for(i = 0; i < num_cars; i++){
        pthread_join(cars[i], NULL);
    }

    free(passengers);
    free(cars);
    free(ticket_queue);
    free(ride_queue);
    printf("Simulation Finished\n");
    return 0;
    
}