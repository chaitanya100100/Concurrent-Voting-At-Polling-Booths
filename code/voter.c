#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include "voting.h"

void * voter_thread(void * args)
{
  Voter * voter = (Voter*)args;
  pthread_mutex_t * mutex_ptr = &(voter->booth->mutex);
  pthread_cond_t * cv_1_ptr = &(voter->booth->cv_1);
  pthread_cond_t * cv_2_ptr = &(voter->booth->cv_2);

  /* Voter waiting to get allocated */
  /* voter_wait_for_evm */
  pthread_mutex_lock(mutex_ptr);
  voter->status = VOTER_WAITING;
  while(voter->status == VOTER_WAITING)
    pthread_cond_wait(cv_1_ptr, mutex_ptr);
  pthread_mutex_unlock(mutex_ptr);

  EVM * evm = voter->evm;

  /* voter is in the slot. waiting for evm to start voting phase. */
  /* voter_in_slot */
  pthread_mutex_lock(mutex_ptr);
  while(evm->flag == 0)
    pthread_cond_wait(cv_1_ptr, mutex_ptr);
  (evm->number_of_slots)--;
  printf("%d booth %d evm %d voter has casted its vote.\n",evm->booth->idx, evm->idx, voter->idx);
  pthread_cond_broadcast(cv_2_ptr);
  pthread_mutex_unlock(mutex_ptr);

  return NULL;
}

Voter* voter_init(Voter * voter, int idx, Booth * booth)
{
  voter = (Voter*)malloc(sizeof(Voter));
  voter->idx = idx;
  voter->booth = booth;
  voter->status = VOTER_CAME;
  voter->evm = NULL;
  return voter;
}
