Main takeaways:

Part 1:
- I started modeling the problem using OOP. It didn't help at all in solving the problem. OOP is overrated (in fact, it sucks. We can chat about that). I only made progress when facing the problem head on as a graph search;
- I used landmarks to break one long BFS into 4 steps. Since BFS branches exponentially, this dramatically reduces the problem's complexity. It is much more efficient to identify landmarks that must be reached, and running BFS between those landmarks, than to run a single BFS;
- I used lookup tables / dynamic dispatch to find out which function to call based on the robot's "action" (<, >, ^, v, A). This is a lot like polymorphism.

Part 2: it's the same problem, but instead of 2 layers of indirection, there are 25. TO DO.
