/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;



int head=0;
int tail=0;
int count=0;

pthread_mutex_t lock;
sem_t sem;

// the work queue
// task worktodo;
task queue[QUEUE_SIZE];

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    //[Contributor B TODO : Producer]
    queue[tail]=t;
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    //[Contributor C TODO : Consumer]
    task t = queue[head];
    return t;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    //[Contributor C TODO : Consumer]
    // execute the task queue
    

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    //[Contributor B TODO : Producer]

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem,0,0);

    for (int i=0; i<NUMBER_OF_THREADS;i++){
        pthread_create(&bee[i],NULL,worker,NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i=0;i< NUMBER_OF_THREADS;i++){
        pthread_cancel(bee[i]);
    }

    for (int i = 0; i< NUMBER_OF_THREADS;i++){
        pthread_join(bee[i],NULL);
    }

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem);
    
}
