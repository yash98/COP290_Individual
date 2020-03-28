# Event Driven Simulation

## Queue approaches

### Single priority queue acc. to occurrence time of events

Items in queue are new structure event

1. Priority queue enables re-entry of items to queue
2. Not multiple queues for each tellers, issues -
   1. Searching queue for previous item in queue when new user enter
   2. If teller on breaker then problem with people in queue

### Set of queues for simultaneous events

Similar as before just parallel instances of queues

1. Array of queues with events, removes problem 2. with last approach

### Single Queue modifies state of environment

State contains other structures which is modified as events occur. Save relevant info in for adding customers and breaks for tellers
