// function protoype
void *do_work(void *arg);

// libraries
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// global varibles
int count = 0;
pthread_mutex_t lock;
pthread_cond_t decremented;

int main(int argc, char const *argv[]) {

  // initalize varibles and threads
  int numberOfThreads = 2;
  long int index;
  pthread_t worker_thread[numberOfThreads];

  // do mutex init here because get errors otherwise
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&decremented, NULL);

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
  if(id == 0)
  {
    // Block until the count is decremented
    pthread_cond_wait(&decremented, &lock);

    count = count + 1;

    pthread_mutex_unlock(&lock);
  }
  else
  {
    // mutual exlusion lock
    pthread_mutex_lock(&lock);

    count = count - 1;

    pthread_mutex_unlock(&lock);

    // signal the other thread to run once decremented
    pthread_cond_signal(&decremented);
  }

}
