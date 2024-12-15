Main takeaways:

Part 1:
- Pretty simple iterative solution. Could be done recursively instead, but the need to mutate only the first and last elements works better with iteration. Recursion would probably mutate them all for naught.

Part 2:
- Recursive check for vertical movement validity ("depth first search");
- Iterative level movement with a queue ordering levels ("breadth first search").
