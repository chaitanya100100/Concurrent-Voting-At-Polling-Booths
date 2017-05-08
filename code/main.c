#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "voting.h"

const int VOTER_CAME=0;
const int VOTER_WAITING=1;
const int VOTER_ASSIGNED=2;
const int VOTER_DONE=3;

int main()
{
  int number_of_booths = 1;

  int _number_of_evms = 4;
  int _max_slots_in_evm = 4;
  int _number_of_voters = 20;

  /* --- input --- */
  //scanf("%d", &number_of_booths);
  int * number_of_evms = (int*)malloc(sizeof(int)*number_of_booths);
  int * max_slots_in_evm = (int*)malloc(sizeof(int)*number_of_booths);
  int * number_of_voters = (int*)malloc(sizeof(int)*number_of_booths);


  for(int i=0; i<number_of_booths; i++)
  {
    //scanf("%d%d%d",&_number_of_voters, &_number_of_evms, &_max_slots_in_evm);
    number_of_voters[i] = _number_of_voters;
    number_of_evms[i] = _number_of_evms;
    max_slots_in_evm[i] = _max_slots_in_evm;
  }


  printf("ELECTION STARTED.\n");

  Booth ** booths = (Booth**)malloc(sizeof(Booth*)*number_of_booths);
  /* booth init */
  for(int i=0; i<number_of_booths; i++)
    booths[i] = booth_init( booths[i], i, number_of_evms[i],
                max_slots_in_evm[i], number_of_voters[i]);

  /* booth thread start */
  for(int i=0; i<number_of_booths; i++)
    pthread_create(&(booths[i]->booth_thread_id),
              NULL, booth_thread, booths[i]);

  /* booth thread joined */
  for(int i=0; i<number_of_booths; i++)
    pthread_join(booths[i]->booth_thread_id, 0);

  printf("ELECTION COMPLETED PEACEFULLY. LONG LIVE DEMOCRACY.\n");

  /* freeing alloted memory */
  free(number_of_voters);
  free(number_of_evms);
  free(max_slots_in_evm);

  for(int i=0; i<number_of_booths; i++)
    free(booths[i]);
  free(booths);

  return 0;
}
