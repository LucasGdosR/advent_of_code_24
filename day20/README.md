Main takeaways:

1. Part 1:
- Python: iterative depth first search for less memory utilization;
- C: even more optimized implementation, but only suitable when only a single path exists. Loss of generality, performance gain;
- Make a first pass of DFS to cache the distance from each tile to the end, leading to a constant time check for cheating shortcuts;
- Python: simplifying edge cases using default values in a dict instead of sentinels;
- C: sentinels simplifying edge cases;
- I could build a data structure to cache all tiles in the path in the C version, but about half the tiles are in the path anyway, so I didn't mind iterating through the whole racetrack again;
- I was lazy and used globals instead of passing pointers around. At least I didn't ship the `goto` version.

2. Part 2:
- Depth-limited breadth-first search;
- Python: playing with condition checking and `cheating_steps` incrementing was enough to speed up the code by 100%. I thought this was remarkable, although it made the code slightly less readable.
