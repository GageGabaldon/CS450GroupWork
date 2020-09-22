#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


struct arguments{
sem_t * sem;
sem_t **allsem;
int * scoreA;
int * scoreB;
int * counterTeamA;
int * counterTeamB;
pthread_mutex_t * scoreLock;
int tid;
int teamNum;
};



//Prototypes
int randThread(int team, int min, int max);
void *do_work(void *arg);



int main(int argc, char *argv) {

  //Seed rng
  //Do not change seed
  srand(42);


  //write code here
  const int NUMBEROFTHREADS = 10;
  int index;
  int team0 = 0;
  int team1 = 1;
  int scoreA = 0;
  int scoreB = 0;
  int counterTeamA = 0;
  int counterTeamB = 0;
  pthread_mutex_t scoreLock;
  pthread_mutex_init(&scoreLock, NULL);
  pthread_t worker_thread[NUMBEROFTHREADS];
  sem_t *sem[NUMBEROFTHREADS];
  struct arguments * args[NUMBEROFTHREADS];


  for (index = 0; index < NUMBEROFTHREADS; index++) {
     sem[index] = calloc(1, sizeof(sem_t));;
     sem_init(sem[index], 0, 0);
  }

  // initialize semaphores and struct
  for (index = 0; index < NUMBEROFTHREADS; index++) {
     args[index] = (struct arguments *)calloc(1, sizeof(struct arguments));
     args[index]->tid = index;
     args[index]->scoreA = &scoreA;
     args[index]->scoreB = &scoreB;
     args[index]->counterTeamA = &counterTeamA;
     args[index]->counterTeamB = &counterTeamB;
     args[index]->scoreLock = &scoreLock;
     args[index]->sem = sem[index];
     args[index]->allsem = sem;

     if (index < 5)
     {
        args[index]->teamNum = team0;
     }
     else
     {
        args[index]->teamNum = team1;
     }
  }

  for (index = 0; index < NUMBEROFTHREADS; index++) {
    if(pthread_create( &worker_thread[index], NULL, do_work,
      (void *)args[index]))
    {
      fprintf(stderr, "error while create thread\n");
      exit(1);
    }
  }
  // joining all the threads together
  for (index = 0; index < NUMBEROFTHREADS; index++) {

    if (pthread_join(worker_thread[index], NULL))
    {
      fprintf(stderr, "Error while joining\n");
      exit(1);
    }
  }

  // freememory
  for (index = 0; index < NUMBEROFTHREADS; index++)
  {
    free(args[index]);
  }

  printf("\n\n");
  exit(0);
}

void *do_work(void *arg) {

   // initalize varibles
  int tid;
  int teamNum;
  int nextid;
  int *scoreA;
  int *scoreB;
  int *counterTeamA;
  int *counterTeamB;
  pthread_mutex_t *lock;
  sem_t *sem;
  sem_t **allsem;
  struct arguments *argument;

  argument = (struct arguments *)arg;

  // assign arguments to local varibles
  tid = argument->tid;
  teamNum = argument->teamNum;
  scoreA = argument->scoreA;
  scoreB = argument->scoreB;
  counterTeamA = argument->counterTeamA;
  counterTeamB = argument->counterTeamB;
  lock = argument->scoreLock;
  sem = argument->sem;
  allsem = argument->allsem;

  // set the first player to start first
  if (tid == 0) {
    sem_post(sem);
  }

  while (*scoreA != 10 && *scoreB != 10) {
     // all threads wait until called upon
     sem_wait(sem);
     nextid=randThread(teamNum, 0, 10);
     // performAction();

     // sem post the thread that nextid is and use the the double pointer to do this
     // if the thread we post to is same a pass if not Interception

     // update the counters as necessary reset counters in Interception
     // if 5 passes are made then increment score

     // ends if any score makes it past threshold

     // check to see if double pointer works


  }

}
//Do not modify
//Team must be 0 or 1
//0 for team 0
//1 for team 1
//min- minimum thread id to generate
//max- maximum thread id to generate
//Min= 0
//Max= 10
//Generated in [0, 10)
int randThread(int team, int min, int max)
{
    double randNum = (double)rand() / (double)RAND_MAX;
    //Generate random semaphore to send the ball/puck/etc. to
    //On the team
    if (randNum<0.6)
    {
      double randNum = ((double)rand() / (double)RAND_MAX)*(max*1.0/2*1.0);
      //send to teammate
      if (team==0)
      {
      return (int) randNum;
      }
      if (team==1)
      {
        return (int)randNum+5;
      }
    }
    //Interception by other team
    else
    {
      double randNum = 5.0*((double)rand() / (double)RAND_MAX);
      //send to other team
      if (team==1)
      {
      return (int) randNum;
      }
      if (team==0)
      {
      return (int)randNum+5;
      }
    }
}
