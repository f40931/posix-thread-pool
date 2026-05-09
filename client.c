/**
 * Example Client Program
 * 
 * This program demonstrates the utilization of the thread pool API with a simple arithmetic task. 
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

// Define data structure for the task
struct data
{
    int a;
    int b;
};

// Function to be executed by the thread pool
void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    // The core logic performed by the worker thread
    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // Create some work to do
    struct data work;
    work.a = 5;
    work.b = 10;

    // Initialize the thread pool
    pool_init();

    // Submit the work to the queue
    pool_submit(&add,&work);

    // Synchronization Delay
    sleep(3);

    pool_shutdown();

    return 0;
}
