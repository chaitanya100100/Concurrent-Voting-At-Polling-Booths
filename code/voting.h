#ifndef ELECTION_HEADER
#define ELECTION_HEADER

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

extern const int VOTER_CAME;
extern const int VOTER_WAITING;
extern const int VOTER_ASSIGNED;
extern const int VOTER_DONE;

struct Booth;
typedef struct Booth Booth;

typedef struct EVM
{
  int idx;
  pthread_t evm_thread_id;
  Booth * booth;
  int number_of_slots;
  int flag;
} EVM;

typedef struct Voter
{
  int idx;
  pthread_t voter_thread_id;
  Booth * booth;
  EVM * evm;
  int status;
} Voter;

struct Booth
{
  int idx;
  pthread_t booth_thread_id;
  int number_of_evms;
  int max_slots_in_evm;
  int number_of_voters;
  int done_voters;
  EVM ** evms;
  Voter ** voters;
  pthread_mutex_t mutex;
  pthread_cond_t cv_1;
  pthread_cond_t cv_2;
};

Booth* booth_init( Booth * booth, int idx,
  int number_of_evms, int max_slots_in_evm, int number_of_voters);
void * booth_thread(void * args);
void booth_cleanup(Booth * booth);

EVM* evm_init(EVM * evm, int idx, Booth * booth);
void * evm_thread(void * args);

Voter* voter_init(Voter * voter, int idx, Booth * booth);
void * voter_thread(void * args);

#endif
