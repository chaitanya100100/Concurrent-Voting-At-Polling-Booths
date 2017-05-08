#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "voting.h"

void * booth_thread(void* args)
{
  Booth * booth = (Booth*)args;
  /* evms and voters init */
  for(int i=0; i<booth->number_of_evms; i++)
    booth->evms[i] = evm_init(booth->evms[i], i, booth);
  for(int i=0; i<booth->number_of_voters; i++)
    booth->voters[i] = voter_init(booth->voters[i], i, booth);

  /* evms and voters threads start */
  for(int i=0; i<booth->number_of_evms; i++)
    pthread_create(&(booth->evms[i]->evm_thread_id),
              NULL, evm_thread, booth->evms[i]);

  for(int i=0; i<booth->number_of_voters; i++)
    pthread_create(&(booth->voters[i]->voter_thread_id),
              NULL, voter_thread, booth->voters[i]);

  /* evms and voters threads joined */
  for(int i=0; i<booth->number_of_evms; i++)
    pthread_join(booth->evms[i]->evm_thread_id, 0);

  for(int i=0; i<booth->number_of_voters; i++)
    pthread_join(booth->voters[i]->voter_thread_id, 0);

  printf("%d booth is signing off.\n", booth->idx);

  /* freeing memory */
  booth_cleanup(booth);
  return NULL;
}

Booth* booth_init( Booth * booth, int idx,
  int number_of_evms, int max_slots_in_evm, int number_of_voters)
{
  booth = (Booth*)malloc(sizeof(Booth));
  booth->idx = idx;
  booth->number_of_evms = number_of_evms;
  booth->max_slots_in_evm = max_slots_in_evm;
  booth->number_of_voters = number_of_voters;
  booth->done_voters = 0;
  booth->evms = (EVM**)malloc(sizeof(EVM*)*number_of_evms);
  booth->voters = (Voter**)malloc(sizeof(Voter*)*number_of_voters);
  pthread_mutex_init(&(booth->mutex), NULL);
  pthread_cond_init(&(booth->cv_1), NULL);
  pthread_cond_init(&(booth->cv_2), NULL);
  return booth;
}

void booth_cleanup(Booth * booth)
{
  for(int i=0; i<booth->number_of_evms; i++)
    free(booth->evms[i]);
  for(int i=0; i<booth->number_of_voters; i++)
    free(booth->voters[i]);
  free(booth->evms);
  free(booth->voters);
}
