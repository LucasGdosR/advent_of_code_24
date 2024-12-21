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
- Circular buffer queue implementation that calculates size based on start/end pointers;
- Chained hash set implementation on top of an arena with 0 collisions for the given input;
- Python: playing with condition checking and `cheating_steps` incrementing was enough to speed up the code by 100%. I thought this was remarkable, although it made the code slightly less readable;
- C: this needed a bunch of sentinels to avoid checking the boundaries, so I added more generic sentinel logic based on macros;
- C: doing some math allowed me to come up with precise limits for data structures. Knowing queues need at most 80 elements, and hash sets need at most 841 elements, was calculated by hand;
- C: trying different hash set sizes and hash functions led to massive reductions in collisions. I was able to reach 0 collisions for my personal input with this implementation, using a prime number for the size, and making each coordinate not interfere with each other;
- C: instead of incrementing and decrementing the queue's size with every enqueue/dequeue operation, I calculated it out of the start/end pointers. This is handy when asking for the length isn't as frequent as enqueuing and dequeuing.
