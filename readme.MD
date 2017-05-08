run :
make
./election

input:

number_of_booths
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 0)
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 1)
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 2)
.
.
.




**Details**

Each booth works independently so the details can be restricted to one booth.

*I have Implemented extra concurrency than asked.
Each evm of booth is getting filled concurrently and also each evm is executing voting phase concurrently.
So there is not any type of blocking during 'some process' like evm filling or voting phase*

**Implementation Details**
I have one mutex and two conditional variables for each booth.

For every voter or evm, if it tries to access shared info, mutex is locked.
Also this mutex is used with two conditional variables.

There are two conditional variables say cv1 and cv2.
cv1 is for evm to wait for some signal from voter.
cv2 is for voter to wait for some signal from evm.

**Process**
1. Voter starts and waits to get assigned.
2. Evm gets free slots and starts assigning voters to itself and signals voters about it.
3. Evm then starts voting phase.
4. Evm signals voters to vote and waits for every assigned voters to vote.
5. Voter, after receiving signal from evm, gives vote and signals evm about it.
6. Evm, after receiving signal from all voters about voting, finishes voting phase and move to next phase.


***It is working without deadlocks even for 2000 voters and takes less than 1 sec to complete. ***
