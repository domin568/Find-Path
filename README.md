# Find path 
Visualisation of taking shortest path in graph using Dijkstra algorithm. To draw graph I used SDL2 and SDL2_ttf (dependencies).
Project works on hardcoded graph but can be easily converted to work on randomly generated graph.

## How to run

Install SDL2 and SDL2_ttf and then:
```
$ make all
./find_path <NodeID1> <NodeID2>
``` 
Makefile uses sdl2-config utility output.

## Example of usage

![](example.gif) 

## TODO
Some refactorization, make it more object oriented
