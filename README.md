# Hexagonal-grid-pathfinding-project

University project involving the creation of a map containing hexagonal cells, the focus of the project is on the implementation of a pathfinding algorithm given two points passed as an input.

## Restrictions
The program must run under certain memory and time limitations, defined by the specifications.
So i applied specific design choices in order to meet those conditions.

For the main pathfinding algorithm i used Dijkstra, the time performance could have been further improved by using other algorithms like Dial. Due to the fact that it is specified that some areas are visited more compared to others, i implemented a cache where i saved the recent executed paths. Memory wise, I used specific variables types for data that data a capped range, organized the structs and used bit masks in order to make them cache friendly.


