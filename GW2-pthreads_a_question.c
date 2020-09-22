#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


struct shared{
int id;
pthread_mutex_t lock;
int *count;
};


void *do_work(void *arg);



int main(int argc, char *argv) {
  int zero = 0;
  int *count = &zero;
  int num_threads=100;

  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);

  pthread_t worker_thread[num_threads];
  struct shared *arg[num_threads];

  // Creating child threads
  int index;

  for (index=0; index < num_threads; index++) {
    arg[index] = (struct shared *)calloc(1, sizeof(struct shared));
    arg[index]->id = index;
    arg[index]->lock = lock;
    arg[index]->count=count;
}

  for (index=0; index < num_threads; index++) {
    if (pthread_create(&worker_thread[index], NULL,
                      do_work, (void *)arg[index])) {
      fprintf(stderr,"Error while creating thread #%d\n",index);
      exit(1);
    }

    fprintf(stdout,"Waiting for child thread to complete\n");
  }

  // Joining with child threads
  for (index=0; index < num_threads; index++) {
    if (pthread_join(worker_thread[index], NULL)) {
      fprintf(stderr,"Error while joining with child thread #%d\n",index);
      exit(1);
    }
  }
  printf("\nCount: %d (should be: %d)", *count, ((num_threads-1)*(num_threads))/2);
  printf("\n\n");
  exit(0);
}


void *do_work(void *arg) {

  struct shared *tid=(struct shared *)arg;

  pthread_mutex_lock(&(tid->lock));
  *tid->count += tid->id;
  pthread_mutex_unlock(&(tid->lock));


  return NULL;
}
