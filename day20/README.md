Main takeaways:

1. Part 1:
- Iterative depth first search for less memory utilization;
- Make a first pass of DFS to cache the distance from each tile to the end, leading to a constant time check for cheating shortcuts;
- Simplifying edge cases using default values in a dict instead of sentinels.

2. Part 2:
- Depth-limited breadth-first search;
- Playing with condition checking and `cheating_steps` incrementing was enough to speed up the code by 100%. I thought this was remarkable, although it made the code slightly less readable.
