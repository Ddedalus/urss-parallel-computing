# Proposal of an unweighted BFS
### to be implemented on a POETS hardware

## Setup
Each agent is mapped to one node (thread?) in an (un)directed, (un)weighted graph.
Node agent knows all its neighbours
Let s denote a source node.

## Agent code

### init
```
list<numeric> neighbours
numeric distance = am I s ?  0 : INF
bool updating = (distance < INF) # are any messages due to be sent?
vote to run
```

### run
```
for each incomming message m:         # update current distance estimate
    if new_distance < distance:
        distance = new_distance
        updating = true

if(updating):
    vote not to halt    # node must be able to change it's mind multiple times during execution as new message arrive
    for each neighbour n:
        send<numeric> distance + 1
    updating = false
else:
    if(distance < INF):
        vote for halt
    else:
        sleep
```

### termination condition:
everyone voted for halt


## Notes on efficiency

### synchronisation
The only synchronisation involved is at determining every node is willing to halt.
This probably involves dedicating some nodes/threads as a hierarchic reporting structure.
It may be beneficial to include certain delay between updating becoming false and voting halt in the node to account for communication delay.
If there is sufficient message inbox functionality and input is available as adjacency list with random access,
it could be even possible to run the algorithm while still loading more remote vertices.

### communication
All messages are sent to neighbours -> are very fast if mapped properly into the hardware.
The only relevant data is distance (32bits?) and receiverID (32bits?), sender ID is not required.
This implementation would send more messages than a classic BFS as there is no guarantee the current estimate possessed by a node is the best
possible (it starts broadcasting it immediately).
Only after a true shortest path is found all subsequent messages will be ignored as providing a worse estimate and eventually halt will be voted.
This may come out to be a bottleneck for vertices with very high degree.


## Developement strategies

### remember neighbours status
Assuming it is possible to convey distance, sender and receiver ID in a single message one could introduce slight modifications to avoid
updating neighbours about whom we know they already had a lower distance estimate:

```
map<numeric, numeric> neighbours -> map<numeric, numeric> neighbours
...
for each incomming message m:
    neighbours[m.sender] = m.distance
    if ...

if(updating):
    vote not to halt
    for each neighbour n:
        if(neighbours[n] > distance + 1)
            send<numeric> distance + 1
...
```
Better yet, instead of using a plain map, one might want to store neighbours in a priority queue
so that the ones with the worst estimate would be informed first.

### split hot nodes
As there is no clock-enforced synchronisation,
one may split any node into an arbitrary number of helper workers connected with zero-weight edges.
When initializing such set of workers, one must just distribute appropriate ID to each neighbour of the original node.
Then, in the messages one would either recognize senders as coworkers or append appropriate boolean flag to all messages.
Likely, performance would benefit from updating fellow workers before anyone else.
With such approach, one can handle arbitrary degree with only a slight performance decrease.

### run multiple searches in parallel
If there is enough memory and messages can store source id, too, one may just keep a list of distance estimates in each node.
This would be more practical for ASP computation as when one BFS is voted as finished,
communication hierarchy may request to free appropriate memory resources and prepare search from another node
while still carying on all other searches.

### weighted search
Is trivially achieved by adding edge weight instead of 1 to the distance received.