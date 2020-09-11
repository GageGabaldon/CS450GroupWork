// function protoype
void *do_work(void *arg);

// libraries
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// global varibles
int count = 0;
pthread_mutex_t lock;

int main(int argc, char const *argv[]) {

  // initalize varibles and threads
  int numberOfThreads = 10;
  long int index;
  pthread_t worker_thread[numberOfThreads];

  // do mutex init here because get errors otherwise
  pthread_mutex_init(&lock, NULL);

  // creating all the threads together
  for (index = 0; index < numberOfThreads; index++)
  {
    if(pthread_create( &worker_thread[index], NULL, *do_work, (void *)index))
    {
      fprintf(stderr, "error while create thread\n");
      exit(1);
    }
  }

  // joining all the threads together
  for (index = 0; index < numberOfThreads; index++)
  {

    if (pthread_join(worker_thread[index], NULL))
    {
      fprintf(stderr, "Error while joining\n");
      exit(1);
    }
  }

  // print count
  printf("The count is: %d\n", count);
  exit(0);
}

void *do_work(void *arg)
{
  // long int to get rid of void casting errors
  long int id;

  id = (long int)arg;

  // if even decrement count if odd increment count
  if(id % 2 == 0)
  {
    // mutual exclusion
    pthread_mutex_lock(&lock);
    count = count - 1;
    pthread_mutex_unlock(&lock);
  }
  else
  {
    // mutual exclusion
    pthread_mutex_lock(&lock);
    count = count + 1;
    pthread_mutex_unlock(&lock);
  }

}
