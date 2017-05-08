#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "voting.h"

void * evm_thread(void * args)
{
  EVM * evm = (EVM*)args;
  Booth * booth = evm->booth;
  pthread_mutex_t * mutex_ptr = &(booth->mutex);
  pthread_cond_t * cv_1_ptr = &(booth->cv_1);
  pthread_cond_t * cv_2_ptr = &(booth->cv_2);

  int i, j, max_;

  while(1)
  {
    pthread_mutex_lock(mutex_ptr);
      if(booth->done_voters == booth->number_of_voters)
      {
        pthread_mutex_unlock(mutex_ptr);
        break;
      }
    pthread_mutex_unlock(mutex_ptr);

    /* evm getting free slots and waiting for voters to get assigned */
    /* polling_ready_evm */
    j = 0;
    max_ = rand()%(booth->max_slots_in_evm) + 1;
    pthread_mutex_lock(mutex_ptr);
    evm->number_of_slots = max_;
    evm->flag = 0;
    pthread_mutex_unlock(mutex_ptr);
    printf("%d booth %d evm %d slots free.\n", booth->idx, evm->idx, max_);

    while(j<max_)
    {
      i = rand()%booth->number_of_voters;
      pthread_mutex_lock(mutex_ptr);
      if(booth->voters[i]->status == VOTER_WAITING)
      {
        booth->voters[i]->status = VOTER_ASSIGNED;
        booth->voters[i]->evm = evm;
        j++;
        (booth->done_voters)++;
        printf("%d booth %d evm %d voter is alloted.\n", booth->idx, evm->idx, i);
      }
      if(booth->done_voters == booth->number_of_voters)
      {
        pthread_mutex_unlock(mutex_ptr);
        break;
      }
      pthread_mutex_unlock(mutex_ptr);
    }

    if(j==0)break;

    /* evm executing voting phase. */
    printf("%d booth %d evm has started voting phase.\n", booth->idx, evm->idx, j);

    pthread_mutex_lock(mutex_ptr);
    evm->number_of_slots = j;
    evm->flag = 1;
    pthread_cond_broadcast(cv_1_ptr);
    while(evm->number_of_slots)
      pthread_cond_wait(cv_2_ptr, mutex_ptr);
    pthread_mutex_unlock(mutex_ptr);

    printf("%d booth %d evm has finished voting phase.\n", booth->idx, evm->idx);
  }
  printf("%d booth %d evm signing off.\n", booth->idx, evm->idx);
  return NULL;
}

EVM* evm_init(EVM * evm, int idx, Booth * booth)
{
  evm = (EVM*)malloc(sizeof(EVM));
  evm->idx = idx;
  evm->booth = booth;
  evm->number_of_slots = 0;
  evm->flag = 0;
  return evm;
}
