# Concurrent Voting At Polling Booth
A concurrent process of voting where the threads representing evms, voters and booths run concurrently using conditional variables and mutex locks.

## Compile and Run
- Compile using simple make and run the executables
- You can change the the way of input and the values of starting variables in `main.c` file
```
make
./election
```
- For alternate method of input, run executable and give input as follows:
```
number_of_booths
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 0)
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 1)
number_of_voters   number_of_evms   max_slots_in_evm    (for booth 2)
.
.
.
```

## Process
Each booth works independently so the process can be restricted to one booth. More details can be found ![here](/problem_statement.pdf).  
1. Voter starts and waits to get assigned.
2. Evm gets free slots and starts assigning voters to itself and signals voters about it.
3. Evm then starts voting phase.
4. Evm signals voters to vote and waits for every assigned voters to vote.
5. Voter, after receiving signal from evm, gives vote and signals evm about it.
6. Evm, after receiving signal from all voters about voting, finishes voting phase and move to next phase.


## Implementation Details
- Each booth works independently so the details can be restricted to one booth.
- Each evm of booth is getting filled concurrently and also each evm is executing voting phase concurrently. So there is not any type of blocking during 'some process' like evm filling or voting phase.
- I have one mutex and two conditional variables for each booth.
- For every voter or evm, if it tries to access shared info, mutex is locked. Also this mutex is used with two conditional variables.
- There are two conditional variables say cv1 and cv2.
	- cv1 is for evm to wait for some signal from voter.
	- cv2 is for voter to wait for some signal from evm.

## Test run
- It is working without deadlocks even for 2000 voters and takes less than 1 sec to complete.
