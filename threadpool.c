/**
 * Thread Pool Implementation
 * 
 * This file implements a fixed-size thread pool using a producer-consumer model.
 * It utilizes POSIX mutexes for synchronization and semaphores for notification.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

// ----------------------------------------------------------------
// Section 1: Constants & Data Structures
// ----------------------------------------------------------------

#define QUEUE_SIZE 10           // maximum number of of pending tasks
#define NUMBER_OF_THREADS 3     // threads pools size
#define TRUE 1
 
// Tasks structure for threads
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// ----------------------------------------------------------------
// Section 2: Shared Resources & Synchronization
// ----------------------------------------------------------------

// Synchronization primitives
pthread_mutex_t lock;
sem_t *sem;

// The work queue
task queue[QUEUE_SIZE];
int head=0;
int tail=0;
int count=0;

pthread_t bee[NUMBER_OF_THREADS];   // Array of worker bee threads

// ----------------------------------------------------------------
// Section 3: Internal Helper Functions (Queue Operations)
// ----------------------------------------------------------------

/**
 * Enqueues a task into the circular buffer.
 * @return 0 if successful, 1 if the queue is full.
 */
int enqueue(task t) 
{
    //[Part B: Producer]
    if(count == QUEUE_SIZE){
        return 1;
    }

    queue[tail]=t;
    tail = (tail + 1) % QUEUE_SIZE;     // Circular buffer logic
    count++;
    return 0;
}

/**
 * Dequeues a task from the circular buffer.
 * @return The task at the head of the queue.
 */
task dequeue() 
{
    //[Part C: Consumer]
    task t = queue[head];
    head = (head + 1) % QUEUE_SIZE;
    count--;
    return t;
}

// ----------------------------------------------------------------
// Section 4: Thread Execution & API Implementation
// ----------------------------------------------------------------

/**
 * The main loop for each worker thread.
 * The worker threads wait for the semaphore, fetch a task, and execute it.
 */
void *worker(void *param)
{
    //[Part C: Consumer]
    // execute the task queue
    while (TRUE) {
        sem_wait(sem);

        pthread_mutex_lock(&lock);
        task t = dequeue();
        pthread_mutex_unlock(&lock);
        
        execute(t.function, t.data);
    }

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
 * Submits work to the pool for asynchronous execution.
 * @return 0 if successful, 1 if the task could not be added.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    //[Part B: Producer]

    task t;
    t.function = somefunction;
    t.data = p;
    
    pthread_mutex_lock(&lock);
    int result = enqueue(t);
    pthread_mutex_unlock(&lock);   

    // Signal a worker thread only if the task was successfully added
    if(result == 0){
        sem_post(sem);
    }

    return result;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&lock, NULL);
    sem = sem_open("/pool_sem", O_CREAT, 0644, 0);

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
    sem_close(sem);
    sem_unlink("/pool_sem");
    
}
