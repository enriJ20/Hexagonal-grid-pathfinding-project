# Hexagonal-grid-pathfinding-project

University project involving the creation of a map containing hexagonal cells, the focus of the project is on the implementation of a pathfinding algorithm given two points as input.

## Restrictions
The program must run under certain memory and time limitations, defined by the software used for grade evaluation.
Therefore, I applied specific design choices in order to meet those conditions.

For the main pathfinding algorithm I used Dijkstra's algorithm with a minqueue, the time performance could have been further improved by using optimizations of this algorithm (for example using a bucket queue). 
Due to the fact that it is specified that some areas are visited more compared to others, I implemented a cache where I saved recently executed paths. 

Memory-wise, I used specific variable types for data that had a capped range, organized the structs and used bit masks in order to make them cache friendly.


