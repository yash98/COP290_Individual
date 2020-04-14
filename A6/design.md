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

### Properties of environment and simulator

1. Time of event is relative to another event exact time is unknown is some cases
2. Unknown time event but order needs to be maintained

### Current Queue Simulator Structure

1. only fifoQueue is being currently used, thinking of appropriate events to model simulation completely
